/*
 * cycle behavior — context-sensitive cycle key.
 * DEMO state: advances demo images.
 * CUSTOM state: cycles pet stat display (char count → happiness → hunger → name).
 * Logic lives in custom_display.c; this file is the thin ZMK behavior driver.
 */

#define DT_DRV_COMPAT zmk_behavior_cycle

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>

void cycle_trigger(void);

static int on_binding_pressed(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
    cycle_trigger();
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_binding_released(struct zmk_behavior_binding *binding,
                                struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_TRANSPARENT;
}

static const struct behavior_driver_api cycle_driver_api = {
    .binding_pressed = on_binding_pressed,
    .binding_released = on_binding_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL,
                         CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                         &cycle_driver_api);
