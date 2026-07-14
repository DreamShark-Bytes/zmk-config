/*
 * Custom display module for Kyria rev3
 * Provides &display_toggle behavior and custom screen switching.
 */

#define DT_DRV_COMPAT zmk_behavior_display_toggle

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <string.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <zmk/display.h>
#include <lvgl.h>

#include "test_image.h"

LOG_MODULE_REGISTER(custom_display, CONFIG_ZMK_LOG_LEVEL);

static lv_obj_t *zmk_screen = NULL;
static lv_obj_t *custom_screen = NULL;
static bool initialized = false;

// Canvas buffer: 128x64 pixels, 1 bit per pixel = 1024 bytes
static uint8_t canvas_buf[128 * 64 / 8];

static void ensure_initialized(void) {
    if (initialized) {
        return;
    }

    zmk_screen = lv_scr_act();

    custom_screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(custom_screen);

    // Copy pixel data from test image, skipping the 8-byte palette
    memcpy(canvas_buf, test_image.data + 8, sizeof(canvas_buf));

    lv_obj_t *canvas = lv_canvas_create(custom_screen);
    lv_canvas_set_buffer(canvas, canvas_buf, 128, 64, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_TOP_LEFT, 0, 0);

    initialized = true;

    if (IS_ENABLED(CONFIG_CUSTOM_DISPLAY_DEFAULT_ON)) {
        lv_scr_load(custom_screen);
    }
}

static void do_toggle(struct k_work *work) {
    ensure_initialized();

    if (lv_scr_act() == custom_screen) {
        lv_scr_load(zmk_screen);
    } else {
        lv_scr_load(custom_screen);
    }
}

K_WORK_DEFINE(toggle_work, do_toggle);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int on_binding_pressed(struct zmk_behavior_binding *binding,
                               struct zmk_behavior_binding_event event) {
    k_work_submit_to_queue(zmk_display_work_q(), &toggle_work);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_binding_released(struct zmk_behavior_binding *binding,
                                struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_TRANSPARENT;
}

static const struct behavior_driver_api display_toggle_driver_api = {
    .binding_pressed = on_binding_pressed,
    .binding_released = on_binding_released,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL, POST_KERNEL,
                         CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                         &display_toggle_driver_api);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
