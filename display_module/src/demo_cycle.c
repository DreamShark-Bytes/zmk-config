/*
 * demo_cycle behavior — cycles through demo images on the custom display screen.
 * Logic lives in custom_display.c; this file is the thin ZMK behavior driver.
 */

#define DT_DRV_COMPAT zmk_behavior_demo_cycle

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>

void demo_cycle_trigger(void);

static int on_binding_pressed(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
    demo_cycle_trigger();
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_binding_released(struct zmk_behavior_binding *binding,
                                struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_TRANSPARENT;
}

static const struct behavior_driver_api demo_cycle_driver_api = {
    .binding_pressed = on_binding_pressed,
    .binding_released = on_binding_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL,
                         CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                         &demo_cycle_driver_api);
