/*
 * bt_switch behavior — disconnect all host profiles, switch to next/prev, then
 * restore the remembered base layer for the new profile.
 *
 * All BLE and keymap logic is central-only (guarded by CONFIG_ZMK_SPLIT_ROLE_CENTRAL).
 * On the peripheral half the behavior compiles but pressing the key is a no-op,
 * which is correct — the peripheral only scans hardware and forwards raw keycodes.
 *
 * Base-layer tracking and NVS persistence are enabled by Kconfig:
 *   CONFIG_BT_SWITCH_PERSIST_BASE_LAYER=y   — track per-profile OS layer; requires
 *                                             base-layers DT property on behavior node
 *   CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE=y — restore last BT profile on boot
 *
 * param1: 0 = next profile, 1 = previous profile
 */

#define DT_DRV_COMPAT zmk_behavior_bt_switch

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

#include <zmk/ble.h>
#include <zmk/keymap.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER) || \
    IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)
#include <zephyr/settings/settings.h>
#endif

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)
#include <zmk/events/ble_active_profile_changed.h>
#endif

#endif /* CONFIG_ZMK_SPLIT_ROLE_CENTRAL */

#include "behavior_bt_switch.h"

#define BT_SWITCH_NEXT 0
#define BT_SWITCH_PREV 1

/* ---- All central-only implementation ---- */

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

/* ---- Settings save work ---- */

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER) || \
    IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)

static void do_settings_save(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(settings_save_work, do_settings_save);

static void bt_switch_schedule_save(void) {
    k_work_reschedule(&settings_save_work, K_SECONDS(2));
}

#endif /* PERSIST_BASE_LAYER || PERSIST_ACTIVE_PROFILE */

/* ---- Active profile storage ---- */

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)
static uint8_t saved_active_profile = 0;
#endif

/* ---- Base layer configuration ---- */

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)

#define BT_SWITCH_BASE_LAYER_ENTRY(node_id, prop, idx) DT_PROP_BY_IDX(node_id, prop, idx),
static const uint8_t base_layers[] = {
    DT_INST_FOREACH_PROP_ELEM(0, base_layers, BT_SWITCH_BASE_LAYER_ENTRY)
};
#define BASE_LAYER_COUNT ((int)ARRAY_SIZE(base_layers))

/* Per-profile index into base_layers[]. Default 0 = first entry (e.g. WINDOWS_L). */
static uint8_t profile_base_layer[ZMK_BLE_PROFILE_COUNT] = {0};

/* Activate the stored base layer for `profile`, deactivate all others in the set.
 *
 * base_layers[0] is the always-on bottom layer (e.g. WINDOWS_L). Deactivating it
 * would break &trans fallthrough, so only indices 1+ are toggled. When stored
 * index is 0, all non-default layers are deactivated and the bottom layer stays
 * active by default. */
static void apply_base_layer(int profile) {
    uint8_t stored_idx = profile_base_layer[profile];
    if (stored_idx >= BASE_LAYER_COUNT) {
        stored_idx = 0;
    }
    for (int i = 1; i < BASE_LAYER_COUNT; i++) {
        if (i == (int)stored_idx) {
            zmk_keymap_layer_activate(base_layers[i]);
        } else {
            zmk_keymap_layer_deactivate(base_layers[i]);
        }
    }
}

#endif /* CONFIG_BT_SWITCH_PERSIST_BASE_LAYER */

/* ---- NVS / Settings ---- */

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER) || \
    IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)

/* Debounced save — used only for base layer (can change quickly on rapid toggles).
 * Active profile is saved immediately in its own event callback. */
static void do_settings_save(struct k_work *work) {
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)
    settings_save_one("bt_layer/profile_layers",
                      profile_base_layer, sizeof(profile_base_layer));
#endif
}

static int bt_layer_settings_set(const char *name, size_t len,
                                  settings_read_cb read_cb, void *cb_arg) {
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)
    if (strcmp(name, "profile_layers") == 0) {
        read_cb(cb_arg, profile_base_layer,
                MIN(len, sizeof(profile_base_layer)));
        return 0;
    }
#endif
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)
    if (strcmp(name, "active_profile") == 0) {
        read_cb(cb_arg, &saved_active_profile,
                MIN(len, sizeof(saved_active_profile)));
        return 0;
    }
#endif
    return 0;
}

SETTINGS_STATIC_HANDLER_DEFINE(bt_layer, "bt_layer", NULL,
                                bt_layer_settings_set, NULL, NULL);

static void do_boot_restore(struct k_work *work) {
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)
    if (saved_active_profile < ZMK_BLE_PROFILE_COUNT &&
        zmk_ble_active_profile_index() != (int)saved_active_profile) {
        zmk_ble_prof_select(saved_active_profile);
    }
#endif
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)
    apply_base_layer(zmk_ble_active_profile_index());
#endif
}
static K_WORK_DELAYABLE_DEFINE(boot_restore_work, do_boot_restore);

static int bt_layer_settings_init(void) {
    settings_load_subtree("bt_layer");
    /* Boot restore runs 100ms later via work queue — after settings_load() and
     * zmk_ble_complete_startup() have both completed in main(). */
    k_work_reschedule(&boot_restore_work, K_MSEC(100));
    return 0;
}
/* Priority 98: loads our NVS data only. All BLE calls deferred to do_boot_restore. */
SYS_INIT(bt_layer_settings_init, APPLICATION, 98);

#endif /* PERSIST_BASE_LAYER || PERSIST_ACTIVE_PROFILE */

/* ---- Active profile event listener ---- */

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)

static int bt_switch_ble_profile_event_cb(const zmk_event_t *eh) {
    saved_active_profile = zmk_ble_active_profile_index();
    /* Save immediately — profile switches are rare (1-byte write) and a
     * debounced save risks losing the update if the keyboard sleeps first. */
    settings_save_one("bt_layer/active_profile",
                      &saved_active_profile, sizeof(saved_active_profile));
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(bt_switch_profile_listener, bt_switch_ble_profile_event_cb);
ZMK_SUBSCRIPTION(bt_switch_profile_listener, zmk_ble_active_profile_changed);

#endif /* PERSIST_ACTIVE_PROFILE */

/* ---- Layer tracking listener ---- */

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)

static int bt_switch_layer_event_cb(const zmk_event_t *eh) {
    int active_profile = zmk_ble_active_profile_index();
    /* Walk indices 1+; if none is active, stored index stays 0 (default). */
    uint8_t new_idx = 0;
    for (int i = 1; i < BASE_LAYER_COUNT; i++) {
        if (zmk_keymap_layer_active(base_layers[i])) {
            new_idx = i;
            break;
        }
    }
    profile_base_layer[active_profile] = new_idx;
    bt_switch_schedule_save();
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(bt_switch_layer_listener, bt_switch_layer_event_cb);
ZMK_SUBSCRIPTION(bt_switch_layer_listener, zmk_layer_state_changed);

#endif /* CONFIG_BT_SWITCH_PERSIST_BASE_LAYER */

#endif /* CONFIG_ZMK_SPLIT_ROLE_CENTRAL */

/* ---- Reset entry point (called by bt_clear; no-op on peripheral) ---- */

void bt_switch_reset_profile_layer(int profile) {
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) && \
    IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)
    if (profile < 0 || profile >= ZMK_BLE_PROFILE_COUNT) {
        return;
    }
    profile_base_layer[profile] = 0;
    bt_switch_schedule_save();
#endif
}

/* ---- Behavior ---- */

static int on_binding_pressed(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    /* Disconnect all bonded profiles before switching.
     *
     * Empirically confirmed: calling disconnect for each profile slot in sequence
     * reliably terminates active BLE links (including iOS/iPadOS) so the departing
     * device's virtual keyboard reappears. Targeted single-profile disconnect (any
     * ordering) did not reliably disconnect iOS in hardware testing — root cause
     * unknown without USB logging. Disconnecting an unconnected slot is a no-op. */
    for (int i = 0; i < ZMK_BLE_PROFILE_COUNT; i++) {
        zmk_ble_prof_disconnect(i);
    }

    if (binding->param1 == BT_SWITCH_PREV) {
        zmk_ble_prof_prev();
    } else {
        zmk_ble_prof_next();
    }

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)
    apply_base_layer(zmk_ble_active_profile_index());
#endif
#endif /* CONFIG_ZMK_SPLIT_ROLE_CENTRAL */
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_binding_released(struct zmk_behavior_binding *binding,
                                struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_TRANSPARENT;
}

static const struct behavior_driver_api bt_switch_driver_api = {
    .binding_pressed  = on_binding_pressed,
    .binding_released = on_binding_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL,
                         CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                         &bt_switch_driver_api);
