/*
 * bt_clear behavior — clear the current BT profile bond and reset its stored
 * base layer to the default.
 *
 * Replaces &bt BT_CLR so that clearing a profile stays consistent with
 * per-profile layer memory in behavior_bt_switch.c.
 *
 * Central-only: on the peripheral half the keypress is a no-op, which is
 * correct — the peripheral only scans hardware and forwards raw keycodes.
 *
 * No parameters (0 binding cells).
 */

#define DT_DRV_COMPAT zmk_behavior_bt_clear

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
#include <zmk/ble.h>
#endif

#include "behavior_bt_switch.h"

static int on_binding_pressed(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    int profile = zmk_ble_active_profile_index();
    bt_switch_reset_profile_layer(profile);
    zmk_ble_prof_disconnect(profile);
    zmk_ble_clear_bonds();
#endif
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_binding_released(struct zmk_behavior_binding *binding,
                                struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_TRANSPARENT;
}

static const struct behavior_driver_api bt_clear_driver_api = {
    .binding_pressed  = on_binding_pressed,
    .binding_released = on_binding_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL,
                         CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                         &bt_clear_driver_api);
