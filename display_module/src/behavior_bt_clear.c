/*
 * bt_clear behavior — clear the current BT profile's bond while also resetting
 * its stored base layer to the default.
 *
 * Replaces ZMK's built-in &bt BT_CLR so that clearing a profile also resets
 * its remembered OS layer. Without this, a cleared profile would still try to
 * restore a stale layer on the next profile switch.
 *
 * No parameters (0 binding cells).
 */

#define DT_DRV_COMPAT zmk_behavior_bt_clear

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/ble.h>

#include "behavior_bt_switch.h"

static int on_binding_pressed(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
    int profile = zmk_ble_active_profile_index();
    bt_switch_reset_profile_layer(profile);
    zmk_ble_prof_disconnect(profile);
    zmk_ble_clear();
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
