/*
 * Custom display module for Kyria rev3.
 * Provides &display_toggle (switch stock ↔ custom screen) and
 * &demo_cycle (step through demo images on the custom screen).
 */

#define DT_DRV_COMPAT zmk_behavior_display_toggle

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <zmk/display.h>
#include <lvgl.h>

#include "demo_list.h"

LOG_MODULE_REGISTER(custom_display, CONFIG_ZMK_LOG_LEVEL);

static lv_obj_t *zmk_screen = NULL;
static lv_obj_t *custom_screen = NULL;
static lv_obj_t *canvas = NULL;
static bool initialized = false;
static int demo_idx = 0;

// Canvas buffer: 128x64 pixels, one lv_color_t per pixel.
// With LV_COLOR_DEPTH=1, sizeof(lv_color_t)=1, so this is 8192 bytes.
static lv_color_t canvas_buf[128 * 64];

// Unpack an INDEXED_1BIT image into canvas_buf and trigger a redraw.
// Inverts bits: SSD1306 has hardware inversion-on, so bit=1 appears dark.
// Negating here means image white (1) → canvas 0 → displays as white.
static void load_image(const lv_img_dsc_t *img) {
    const uint8_t *packed = img->data + 8; // skip 8-byte INDEXED_1BIT palette
    int w = img->header.w;
    int h = img->header.h;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int idx = y * w + x;
            uint8_t bit = (packed[idx / 8] >> (7 - (idx % 8))) & 1;
            canvas_buf[idx].full = bit ? 0 : 1;
        }
    }
    if (canvas) {
        lv_obj_invalidate(canvas);
    }
}

static void ensure_initialized(void) {
    if (initialized) {
        return;
    }

    zmk_screen = lv_scr_act();

    custom_screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(custom_screen);

    load_image(demo_images[0]);

    canvas = lv_canvas_create(custom_screen);
    lv_canvas_set_buffer(canvas, canvas_buf, 128, 64, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_TOP_LEFT, 0, 0);

    initialized = true;

    if (IS_ENABLED(CONFIG_CUSTOM_DISPLAY_DEFAULT_ON)) {
        lv_scr_load(custom_screen);
    }
}

// --- display_toggle behavior ---

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

// --- demo_cycle trigger (called from demo_cycle.c) ---

static void do_demo_cycle(struct k_work *work) {
    ensure_initialized();
    demo_idx = (demo_idx + 1) % demo_image_count;
    load_image(demo_images[demo_idx]);
}

K_WORK_DEFINE(demo_cycle_work, do_demo_cycle);

void demo_cycle_trigger(void) {
    k_work_submit_to_queue(zmk_display_work_q(), &demo_cycle_work);
}
