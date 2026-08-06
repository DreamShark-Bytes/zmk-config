/*
 * bt_switch behavior — disconnect all host profiles then advance to next/prev.
 *
 * ZMK's BT_NXT / BT_PRV only change the active input target; they do not
 * terminate the BLE connection to the previous host. This leaves the old device
 * believing a keyboard is connected, which suppresses its software keyboard.
 *
 * This behavior disconnects every profile slot before switching so the departing
 * host gets a proper BLE disconnect and its virtual keyboard reappears.
 * Disconnecting an already-disconnected slot is a no-op (-ENODEV), so the loop
 * is safe regardless of how many devices are currently connected.
 *
 * param1: 0 = next profile, 1 = previous profile
 */

#define DT_DRV_COMPAT zmk_behavior_bt_switch

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#include <zmk/ble.h>

#define BT_SWITCH_NEXT 0
#define BT_SWITCH_PREV 1

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
