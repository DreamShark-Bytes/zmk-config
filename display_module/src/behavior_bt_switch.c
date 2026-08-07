/*
 * bt_switch behavior — disconnect all host profiles, switch to next/prev, then
 * restore the remembered base layer for the new profile.
 *
 * Base layers are a mutually exclusive set of OS/layout layers (e.g. WINDOWS_L
 * and MAC_L). Exactly one is considered "active" as the default at any time.
 * Configure the set via the `base-layers` DT property on the behavior node:
 *
 *   bt_switch: bt_switch {
 *       compatible = "zmk,behavior-bt-switch";
 *       #binding-cells = <1>;
 *       base-layers = <WINDOWS_L MAC_L>;
 *   };
 *
 * Omit `base-layers` entirely to disable per-profile layer tracking (behavior
 * acts as a simple "disconnect all + switch" operation).
 *
 * Per-profile memory is in RAM (resets on power cycle). Enable NVS persistence
 * in kyria_rev3.conf:
 *   CONFIG_BT_SWITCH_PERSIST_BASE_LAYER=y      — persist per-profile base layer
 *   CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE=y  — restore last BT profile on boot
 *
 * param1: 0 = next profile, 1 = previous profile
 */

#define DT_DRV_COMPAT zmk_behavior_bt_switch

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
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

#include "behavior_bt_switch.h"

#define BT_SWITCH_NEXT 0
#define BT_SWITCH_PREV 1

/* ---- Settings save (defined early so later code can call it) ---- */

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

/* ---- Base layer configuration (from DT) ---- */

#if DT_INST_HAS_PROP(0, base_layers)

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
 * would break &trans fallthrough, so we only toggle indices 1 and above. When the
 * stored index is 0, all non-default layers are deactivated and the bottom layer
 * remains active by default. */
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

#endif /* DT_INST_HAS_PROP(0, base_layers) */

/* ---- Reset entry point (called by bt_clear behavior) ---- */

void bt_switch_reset_profile_layer(int profile) {
#if DT_INST_HAS_PROP(0, base_layers)
    if (profile < 0 || profile >= ZMK_BLE_PROFILE_COUNT) {
        return;
    }
    profile_base_layer[profile] = 0;
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)
    bt_switch_schedule_save();
#endif
#endif
}

/* ---- NVS / Settings ---- */

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER) || \
    IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)

static void do_settings_save(struct k_work *work) {
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER) && DT_INST_HAS_PROP(0, base_layers)
    settings_save_one("bt_layer/profile_layers",
                      profile_base_layer, sizeof(profile_base_layer));
#endif
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)
    settings_save_one("bt_layer/active_profile",
                      &saved_active_profile, sizeof(saved_active_profile));
#endif
}

static int bt_layer_settings_set(const char *name, size_t len,
                                  settings_read_cb read_cb, void *cb_arg) {
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER) && DT_INST_HAS_PROP(0, base_layers)
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

static int bt_layer_settings_init(void) {
    settings_load_subtree("bt_layer");
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)
    if (saved_active_profile < ZMK_BLE_PROFILE_COUNT) {
        zmk_ble_prof_select(saved_active_profile);
    }
#endif
#if DT_INST_HAS_PROP(0, base_layers)
    apply_base_layer(zmk_ble_active_profile_index());
#endif
    return 0;
}
/* Priority 98: after ZMK BLE init (~90) and Zephyr settings subsystem init (~95). */
SYS_INIT(bt_layer_settings_init, APPLICATION, 98);

#endif /* PERSIST_BASE_LAYER || PERSIST_ACTIVE_PROFILE */

/* ---- Active profile event listener ---- */

#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_ACTIVE_PROFILE)

static int bt_switch_ble_profile_event_cb(const zmk_event_t *eh) {
    saved_active_profile = zmk_ble_active_profile_index();
    bt_switch_schedule_save();
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(bt_switch_profile_listener, bt_switch_ble_profile_event_cb);
ZMK_SUBSCRIPTION(bt_switch_profile_listener, zmk_ble_active_profile_changed);

#endif /* PERSIST_ACTIVE_PROFILE */

/* ---- Layer tracking listener ---- */

#if DT_INST_HAS_PROP(0, base_layers)

static int bt_switch_layer_event_cb(const zmk_event_t *eh) {
    int active_profile = zmk_ble_active_profile_index();
    /* Walk indices 1+; if none is active, the stored index stays 0 (default). */
    uint8_t new_idx = 0;
    for (int i = 1; i < BASE_LAYER_COUNT; i++) {
        if (zmk_keymap_layer_active(base_layers[i])) {
            new_idx = i;
            break;
        }
    }
    profile_base_layer[active_profile] = new_idx;
#if IS_ENABLED(CONFIG_BT_SWITCH_PERSIST_BASE_LAYER)
    bt_switch_schedule_save();
#endif
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(bt_switch_layer_listener, bt_switch_layer_event_cb);
ZMK_SUBSCRIPTION(bt_switch_layer_listener, zmk_layer_state_changed);

#endif /* DT_INST_HAS_PROP(0, base_layers) */

/* ---- Behavior ---- */

static int on_binding_pressed(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
    for (int i = 0; i < ZMK_BLE_PROFILE_COUNT; i++) {
        zmk_ble_prof_disconnect(i);
    }

    if (binding->param1 == BT_SWITCH_PREV) {
        zmk_ble_prof_prev();
    } else {
        zmk_ble_prof_next();
    }

#if DT_INST_HAS_PROP(0, base_layers)
    apply_base_layer(zmk_ble_active_profile_index());
#endif
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
