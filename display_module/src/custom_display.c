/*
 * Custom display module for Kyria rev3.
 *
 * Three display states cycled by &display_toggle:
 *   STOCK  — ZMK built-in screen (BT, battery, layer number)
 *   CUSTOM — Real layout: info column + pet area
 *   DEMO   — Canvas showing mockup images; advance with &demo_cycle
 *
 * Configure layout, fonts, and layer names in display_config.h.
 * Wire live data (battery %, layer index, BT status) in the
 * "DATA CALLBACKS — Phase 2" section below.
 */

#define DT_DRV_COMPAT zmk_behavior_display_toggle

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <zmk/display.h>
#include <lvgl.h>

#include "display_config.h"
#include "demo_list.h"

#include <stdio.h>
#include "icon_link.h"
#include "icon_link_broken.h"
#include "icon_bt.h"
#include "icon_check.h"
#include "icon_x.h"
#include "icon_battery.h"
#include "icon_lightning.h"
#include "icon_os_mac.h"
#include "icon_os_windows.h"
#include "pet_temp_image.h"

LOG_MODULE_REGISTER(custom_display, CONFIG_ZMK_LOG_LEVEL);

// ---------------------------------------------------------------------------
// Layer names (defined via macro in display_config.h)
// ---------------------------------------------------------------------------
static const char *const layer_names[LAYER_NAME_COUNT] __attribute__((unused)) = { LAYER_NAMES_LIST };

// ---------------------------------------------------------------------------
// Display state
// ---------------------------------------------------------------------------
typedef enum {
    DISPLAY_STATE_STOCK = 0,
    DISPLAY_STATE_CUSTOM,
    DISPLAY_STATE_DEMO,
    DISPLAY_STATE_COUNT,
} display_state_t;

static display_state_t current_state = DISPLAY_STATE_STOCK;
static bool initialized = false;

// ---------------------------------------------------------------------------
// Screens
// ---------------------------------------------------------------------------
static lv_obj_t *zmk_screen  = NULL;  // ZMK stock screen (reference only)
static lv_obj_t *real_screen = NULL;  // CUSTOM state
static lv_obj_t *demo_screen = NULL;  // DEMO state

// Demo canvas
static lv_color_t canvas_buf[DISPLAY_WIDTH * DISPLAY_HEIGHT];
static lv_obj_t *canvas = NULL;
static int demo_idx = 0;

// ---------------------------------------------------------------------------
// Widget handles — updated by data callbacks (Phase 2)
// ---------------------------------------------------------------------------
static lv_obj_t *w_link_icon     = NULL;
static lv_obj_t *w_bt_icon       = NULL;
static lv_obj_t *w_bt_profile    = NULL;
static lv_obj_t *w_bt_conn_icon  = NULL;
static lv_obj_t *w_battery_icon  = NULL;
static lv_obj_t *w_battery_pct   = NULL;
static lv_obj_t *w_os_icon       = NULL;
static lv_obj_t *w_layer_l       = NULL;
static lv_obj_t *w_layer_colon   = NULL;
static lv_obj_t *w_layer_name    = NULL;
static lv_obj_t *w_status        = NULL;

// ---------------------------------------------------------------------------
// Layout helpers
// ---------------------------------------------------------------------------
static lv_obj_t *make_img(lv_obj_t *parent, const lv_img_dsc_t *src, int x, int y) {
    lv_obj_t *img = lv_img_create(parent);
    lv_img_set_src(img, src);
    lv_obj_set_pos(img, x, y);
    return img;
}

static lv_obj_t *make_label(lv_obj_t *parent, const lv_font_t *font,
                             const char *text, int x, int y) {
    lv_obj_t *lbl = lv_label_create(parent);
    lv_obj_set_style_text_font(lbl, font, 0);
    lv_label_set_text(lbl, text);
    lv_obj_set_pos(lbl, x, y);
    return lbl;
}

// ---------------------------------------------------------------------------
// Demo screen (canvas-based, for mockup images)
// ---------------------------------------------------------------------------
static void load_demo_image(const lv_img_dsc_t *img) {
    const uint8_t *packed = img->data + 8;
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

static void build_demo_screen(void) {
    demo_screen = lv_obj_create(NULL);
    lv_obj_remove_style_all(demo_screen);
    load_demo_image(demo_images[0]);
    canvas = lv_canvas_create(demo_screen);
    lv_canvas_set_buffer(canvas, canvas_buf, DISPLAY_WIDTH, DISPLAY_HEIGHT,
                         LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_TOP_LEFT, 0, 0);
}

// ---------------------------------------------------------------------------
// Real layout screen — info column (left) + pet area (right)
// Placeholder values shown until Phase 2 data callbacks are wired in.
// ---------------------------------------------------------------------------
static void build_real_screen(void) {
    real_screen = lv_obj_create(NULL);

    int x = -1;
    w_link_icon = make_img(real_screen, &icon_link, x, ROW_TOP_Y);
    x = -1 + 13 + ICON_TEXT_GAP;
    w_bt_icon = make_img(real_screen, &icon_bt, x, ROW_TOP_Y);
    x += 13 + ICON_TEXT_GAP;
    w_bt_profile = make_label(real_screen, FONT_BT_PROFILE, "1", x, ROW_TOP_Y);
    x += 9 + ICON_TEXT_GAP;
    w_bt_conn_icon = make_img(real_screen, &icon_check, x, ROW_TOP_Y);

    x = -1;
    w_battery_icon = make_img(real_screen, &icon_battery, x, ROW_BATTERY_Y);
    x = -1 + 13 + ICON_TEXT_GAP;
    w_battery_pct = make_label(real_screen, FONT_BATTERY_NUM, "99%", x, ROW_BATTERY_Y);

    x = -1;
    w_os_icon = make_img(real_screen, &icon_os_windows, x, ROW_LAYER_Y);
    x = -1 + 13 + ICON_TEXT_GAP;
    w_layer_l = make_label(real_screen, FONT_LAYER_L, "L", x, ROW_LAYER_Y);
    x += 10;
    w_layer_colon = make_label(real_screen, FONT_LAYER_COLON, ":",
                               x, ROW_LAYER_Y + LAYER_COLON_Y_OFFSET);
    x += 4;
    int name_max_w = PET_AREA_X - x - 1;
    w_layer_name = make_label(real_screen, FONT_LAYER_NAME, "BASE",
                              x, ROW_LAYER_Y + LAYER_NAME_Y_OFFSET);
    lv_obj_set_width(w_layer_name, name_max_w);
    lv_label_set_long_mode(w_layer_name, LV_LABEL_LONG_CLIP);

    int status_y = DISPLAY_HEIGHT - 13;
    w_status = make_label(real_screen, FONT_STATUS_TEXT,
                          STATUS_ICON_CURRENCY "0", -1, status_y);
    lv_obj_set_width(w_status, PET_AREA_X + 1);
    lv_label_set_long_mode(w_status, LV_LABEL_LONG_CLIP);
    // lv_label_set_long_mode(w_status, LV_LABEL_LONG_SCROLL_CIRCULAR);
    // lv_obj_set_style_anim_speed(w_status, STATUS_MARQUEE_SPEED, 0);

    lv_obj_t *pet_container = lv_obj_create(real_screen);
    lv_obj_remove_style_all(pet_container);
    lv_obj_set_pos(pet_container, PET_AREA_X, PET_AREA_Y);
    lv_obj_set_size(pet_container, PET_AREA_WIDTH, PET_AREA_HEIGHT);
    lv_obj_t *pet_img = lv_img_create(pet_container);
    lv_img_set_src(pet_img, &pet_temp_image);
    lv_obj_set_pos(pet_img, 0, 0);
}

// ---------------------------------------------------------------------------
// Initialization
// ---------------------------------------------------------------------------
static void ensure_initialized(void) {
    if (initialized) return;

    zmk_screen = lv_scr_act();
    build_demo_screen();
    build_real_screen();
    initialized = true;

    if (IS_ENABLED(CONFIG_CUSTOM_DISPLAY_DEFAULT_ON)) {
        lv_scr_load(real_screen);
        current_state = DISPLAY_STATE_CUSTOM;
    }
}

// ---------------------------------------------------------------------------
// State transitions
// ---------------------------------------------------------------------------
static void set_display_state(display_state_t state) {
    current_state = state;
    switch (state) {
    case DISPLAY_STATE_STOCK:
        lv_scr_load(zmk_screen);
        break;
    case DISPLAY_STATE_CUSTOM:
        lv_scr_load(real_screen);
        break;
    case DISPLAY_STATE_DEMO:
        lv_scr_load(demo_screen);
        break;
    default:
        break;
    }
}

// ---------------------------------------------------------------------------
// DATA CALLBACKS — Phase 2
// Uncomment and implement to wire live ZMK state to the layout widgets.
// ---------------------------------------------------------------------------

// void custom_display_set_layer(uint8_t layer_idx) {
//     if (!initialized) return;
//     char num_buf[4];
//     const char *name = (layer_idx < LAYER_NAME_COUNT) ? layer_names[layer_idx] : NULL;
//     if (name) {
//         lv_label_set_text(w_layer_name, name);
//     } else {
//         snprintf(num_buf, sizeof(num_buf), "%d", layer_idx);
//         lv_label_set_text(w_layer_name, num_buf);
//     }
// }

// void custom_display_set_battery(uint8_t pct, bool charging) {
//     if (!initialized) return;
//     char buf[6];
//     snprintf(buf, sizeof(buf), "%d%%", pct);
//     lv_label_set_text(w_battery_pct, buf);
//     lv_img_set_src(w_battery_icon, charging ? &icon_lightning : &icon_battery);
// }

// void custom_display_set_bt(uint8_t profile, bool connected) {
//     if (!initialized) return;
//     char buf[2] = { '1' + profile, '\0' };
//     lv_label_set_text(w_bt_profile, buf);
//     lv_img_set_src(w_bt_conn_icon, connected ? &icon_check : &icon_x);
// }

// void custom_display_set_split_connected(bool connected) {
//     if (!initialized) return;
//     lv_img_set_src(w_link_icon, connected ? &icon_link : &icon_link_broken);
// }

// void custom_display_set_status(const char *text) {
//     if (!initialized) return;
//     lv_label_set_text(w_status, text);
// }

// ---------------------------------------------------------------------------
// display_toggle behavior
// ---------------------------------------------------------------------------
static void do_toggle(struct k_work *work) {
    ensure_initialized();
    display_state_t next = (current_state + 1) % DISPLAY_STATE_COUNT;
    set_display_state(next);
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

// ---------------------------------------------------------------------------
// demo_cycle trigger (called from demo_cycle.c)
// ---------------------------------------------------------------------------
static void do_demo_cycle(struct k_work *work) {
    ensure_initialized();
    if (current_state != DISPLAY_STATE_DEMO) return;
    demo_idx = (demo_idx + 1) % demo_image_count;
    load_demo_image(demo_images[demo_idx]);
}

K_WORK_DEFINE(demo_cycle_work, do_demo_cycle);

void demo_cycle_trigger(void) {
    k_work_submit_to_queue(zmk_display_work_q(), &demo_cycle_work);
}
