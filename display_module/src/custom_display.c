/*
 * Custom display module for Kyria rev3.
 *
 * Three display states cycled by &display_toggle:
 *   STOCK  — ZMK built-in screen (BT, battery, layer number)
 *   CUSTOM — Real layout: info column (left) + pet area (right)
 *   DEMO   — Canvas showing mockup images; advance with &cycle
 *
 * Configure layout, fonts, and layer names in display_config.h.
 * Wire live data (battery %, layer index, BT status) in the
 * "DATA CALLBACKS — Phase 2" section below.
 * Pet state (char count, happiness, hunger, name) lives in virtual_pet.c.
 */

#define DT_DRV_COMPAT zmk_behavior_display_toggle

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/battery.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/usb.h>
#include <zmk/events/split_peripheral_status_changed.h>
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/ble.h>
#endif
#include <lvgl.h>

#include "display_config.h"
#include "demo_list.h"
#include "virtual_pet.h"

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
static lv_obj_t *w_status_icon   = NULL;  // pet stat icon — content set by virtual_pet
static lv_obj_t *w_status_label  = NULL;  // pet stat value — content set by virtual_pet

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
// Real layout screen
// Central (left): full info column — BT, battery, layer, pet stats, pet area.
// Peripheral (right): split connection icon + battery (pet display — Phase 2).
// Placeholder values shown until Phase 2 data callbacks are wired in.
// Status row (bottom-left) and pet container are handed to virtual_pet after
// widget creation — virtual_pet owns all content within them.
// ---------------------------------------------------------------------------
static void build_real_screen(void) {
    real_screen = lv_obj_create(NULL);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    // Right half — split connection + battery until pet is implemented.
    // Start disconnected; zmk_split_peripheral_status_changed fires when link comes up.
    w_link_icon = make_img(real_screen, &icon_link_broken, -1, ROW_TOP_Y);
    w_battery_icon = make_img(real_screen, &icon_battery, -1, ROW_BATTERY_Y);
    w_battery_pct = make_label(real_screen, FONT_BATTERY_NUM, "99%",
                               -1 + 13 + ICON_TEXT_GAP, ROW_BATTERY_Y + LARGE_FONT_Y_OFFSET);
    return;
#endif

    int x = -1;
    w_link_icon = make_img(real_screen, &icon_link_broken, x, ROW_TOP_Y);
    x = -1 + 13 + ICON_TEXT_GAP;
    w_bt_icon = make_img(real_screen, &icon_bt, x, ROW_TOP_Y);
    x += 13 + ICON_TEXT_GAP;
    w_bt_profile = make_label(real_screen, FONT_BT_PROFILE, "1", x, ROW_TOP_Y + LARGE_FONT_Y_OFFSET);
    x += 9 + ICON_TEXT_GAP;
    w_bt_conn_icon = make_img(real_screen, &icon_check, x, ROW_TOP_Y);

    x = -1;
    w_battery_icon = make_img(real_screen, &icon_battery, x, ROW_BATTERY_Y);
    x = -1 + 13 + ICON_TEXT_GAP;
    w_battery_pct = make_label(real_screen, FONT_BATTERY_NUM, "99%", x, ROW_BATTERY_Y + LARGE_FONT_Y_OFFSET);

    x = -1;
    w_os_icon = make_img(real_screen, &icon_os_windows, x, ROW_LAYER_Y);
    x = -1 + 13 + ICON_TEXT_GAP;
    w_layer_l = make_label(real_screen, FONT_LAYER_L, "L", x, ROW_LAYER_Y + LARGE_FONT_Y_OFFSET);
    x += 10;
    w_layer_colon = make_label(real_screen, FONT_LAYER_COLON, ":",
                               x, ROW_LAYER_Y + LAYER_COLON_Y_OFFSET);
    x += 4;
    int name_max_w = PET_AREA_X - x - 1;
    w_layer_name = make_label(real_screen, FONT_LAYER_NAME, "BASE",
                              x, ROW_LAYER_Y + LAYER_NAME_Y_OFFSET);
    lv_obj_set_width(w_layer_name, name_max_w);
    lv_label_set_long_mode(w_layer_name, LV_LABEL_LONG_CLIP);

    // Status row — icon shell + label shell; virtual_pet owns the content.
    int status_y = DISPLAY_HEIGHT - 13;
    w_status_icon = lv_img_create(real_screen);
    lv_obj_set_pos(w_status_icon, -1, status_y);
    w_status_label = make_label(real_screen, FONT_STATUS_TEXT, "",
                                -1 + 13 + ICON_TEXT_GAP, status_y + LARGE_FONT_Y_OFFSET);

    // Pet container — virtual_pet owns all rendering inside it.
    lv_obj_t *pet_container = lv_obj_create(real_screen);
    lv_obj_remove_style_all(pet_container);
    lv_obj_set_style_border_width(pet_container, 0, 0);
    lv_obj_set_style_bg_opa(pet_container, LV_OPA_TRANSP, 0);
    lv_obj_set_scrollbar_mode(pet_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_pos(pet_container, PET_AREA_X, PET_AREA_Y);
    lv_obj_set_size(pet_container, PET_AREA_WIDTH, PET_AREA_HEIGHT);

    virtual_pet_init(pet_container, w_status_icon, w_status_label);
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
// Peripheral auto-init
// Behaviors run on the central only, so the peripheral's ensure_initialized()
// is never triggered by a key press. Instead we auto-init at startup via two
// chained work items: a delayable on the system queue (guarantees ZMK's
// display thread is running before we touch LVGL), then one on the display
// work queue (ensures LVGL calls are on the correct thread).
// ---------------------------------------------------------------------------
#if !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

static void do_peripheral_display_init(struct k_work *work) {
    ensure_initialized();
    lv_scr_load(real_screen);
    current_state = DISPLAY_STATE_CUSTOM;
}
K_WORK_DEFINE(peripheral_display_work, do_peripheral_display_init);

static void submit_peripheral_display_init(struct k_work *work) {
    k_work_submit_to_queue(zmk_display_work_q(), &peripheral_display_work);
}
K_WORK_DELAYABLE_DEFINE(peripheral_display_delayed, submit_peripheral_display_init);

static int peripheral_display_sys_init(const struct device *dev) {
    k_work_schedule(&peripheral_display_delayed, K_MSEC(50));
    return 0;
}
SYS_INIT(peripheral_display_sys_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);

// --- Split link icon: live connection status ---
static bool pending_split_connected = false;

static void do_update_split_link(struct k_work *work) {
    if (!initialized || !w_link_icon) return;
    lv_img_set_src(w_link_icon, pending_split_connected ? &icon_link : &icon_link_broken);
}
K_WORK_DEFINE(update_split_link_work, do_update_split_link);

static int split_status_event_cb(const zmk_event_t *eh) {
    const struct zmk_split_peripheral_status_changed *ev =
        as_zmk_split_peripheral_status_changed(eh);
    if (ev) {
        pending_split_connected = ev->connected;
        k_work_submit_to_queue(zmk_display_work_q(), &update_split_link_work);
    }
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(display_split_listener, split_status_event_cb);
ZMK_SUBSCRIPTION(display_split_listener, zmk_split_peripheral_status_changed);

#endif /* !IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) */

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
// Live ZMK state → layout widgets via event subscriptions.
// All LVGL updates run on the display work queue.
// ---------------------------------------------------------------------------

// --- Battery: both halves report their own local battery ---

static uint8_t pending_battery_pct = 0;

static void do_update_battery(struct k_work *work) {
    if (!initialized || !w_battery_pct) return;
    char buf[5];
    snprintf(buf, sizeof(buf), "%d%%", pending_battery_pct);
    lv_label_set_text(w_battery_pct, buf);
}
K_WORK_DEFINE(update_battery_work, do_update_battery);

static int battery_event_cb(const zmk_event_t *eh) {
    pending_battery_pct = zmk_battery_state_of_charge();
    k_work_submit_to_queue(zmk_display_work_q(), &update_battery_work);
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(display_battery_listener, battery_event_cb);
ZMK_SUBSCRIPTION(display_battery_listener, zmk_battery_state_changed);

// --- USB charging icon: both halves ---

static void do_update_charging(struct k_work *work) {
    if (!initialized || !w_battery_icon) return;
    bool charging = zmk_usb_get_conn_state() != ZMK_USB_CONN_NONE;
    lv_img_set_src(w_battery_icon, charging ? &icon_lightning : &icon_battery);
}
K_WORK_DEFINE(update_charging_work, do_update_charging);

static int usb_conn_event_cb(const zmk_event_t *eh) {
    k_work_submit_to_queue(zmk_display_work_q(), &update_charging_work);
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(display_usb_listener, usb_conn_event_cb);
ZMK_SUBSCRIPTION(display_usb_listener, zmk_usb_conn_state_changed);

// --- Layer name + OS icon, BT profile: central only ---

#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

static uint8_t pending_layer      = 0;
static bool    pending_mac_active = false;

static void do_update_layer(struct k_work *work) {
    if (!initialized) return;
    lv_img_set_src(w_os_icon, pending_mac_active ? &icon_os_mac : &icon_os_windows);
    uint8_t layer = pending_layer;
    const char *name = (layer < LAYER_NAME_COUNT) ? layer_names[layer] : NULL;
    if (name) {
        lv_label_set_text(w_layer_name, name);
    } else {
        char buf[4];
        snprintf(buf, sizeof(buf), "%d", layer);
        lv_label_set_text(w_layer_name, buf);
    }
}
K_WORK_DEFINE(update_layer_work, do_update_layer);

static int layer_event_cb(const zmk_event_t *eh) {
    pending_layer      = zmk_keymap_highest_layer_active();
    pending_mac_active = zmk_keymap_layer_active(LAYER_IDX_MAC);
    k_work_submit_to_queue(zmk_display_work_q(), &update_layer_work);
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(display_layer_listener, layer_event_cb);
ZMK_SUBSCRIPTION(display_layer_listener, zmk_layer_state_changed);

static uint8_t pending_bt_profile   = 0;
static bool    pending_bt_connected = false;

static void do_update_bt(struct k_work *work) {
    if (!initialized) return;
    char buf[2] = { '1' + pending_bt_profile, '\0' };
    lv_label_set_text(w_bt_profile, buf);
    lv_img_set_src(w_bt_conn_icon, pending_bt_connected ? &icon_check : &icon_x);
}
K_WORK_DEFINE(update_bt_work, do_update_bt);

static int ble_event_cb(const zmk_event_t *eh) {
    pending_bt_profile   = zmk_ble_active_profile_index();
    pending_bt_connected = zmk_ble_active_profile_is_connected();
    k_work_submit_to_queue(zmk_display_work_q(), &update_bt_work);
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(display_ble_listener, ble_event_cb);
ZMK_SUBSCRIPTION(display_ble_listener, zmk_ble_active_profile_changed);

// --- Split link icon: live connection status (central side) ---
static bool pending_central_split_connected = false;

static void do_update_central_split_link(struct k_work *work) {
    if (!initialized || !w_link_icon) return;
    lv_img_set_src(w_link_icon, pending_central_split_connected ? &icon_link : &icon_link_broken);
}
K_WORK_DEFINE(update_central_split_link_work, do_update_central_split_link);

static int central_split_status_cb(const zmk_event_t *eh) {
    const struct zmk_split_peripheral_status_changed *ev =
        as_zmk_split_peripheral_status_changed(eh);
    if (ev) {
        pending_central_split_connected = ev->connected;
        k_work_submit_to_queue(zmk_display_work_q(), &update_central_split_link_work);
    }
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(display_central_split_listener, central_split_status_cb);
ZMK_SUBSCRIPTION(display_central_split_listener, zmk_split_peripheral_status_changed);

#endif /* IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) */

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
// cycle trigger (called from cycle.c)
// Context-sensitive: DEMO advances mockup images; CUSTOM advances pet stats.
// ---------------------------------------------------------------------------
static void do_cycle(struct k_work *work) {
    ensure_initialized();
    switch (current_state) {
    case DISPLAY_STATE_DEMO:
        demo_idx = (demo_idx + 1) % demo_image_count;
        load_demo_image(demo_images[demo_idx]);
        break;
    case DISPLAY_STATE_CUSTOM:
        virtual_pet_cycle_stat();
        break;
    default:
        break;
    }
}

K_WORK_DEFINE(cycle_work, do_cycle);

void cycle_trigger(void) {
    k_work_submit_to_queue(zmk_display_work_q(), &cycle_work);
}
