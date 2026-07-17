/*
 * virtual_pet.c — pet state, stat cycling, character counter, pet area rendering.
 *
 * Stat modes (cycled by &cycle in CUSTOM display state):
 *   STAT_CHAR_COUNT  — keypresses this session, shown with coin icon
 *   STAT_HAPPINESS   — placeholder until pet engine exists
 *   STAT_HUNGER      — placeholder until pet engine exists
 *   STAT_NAME        — placeholder until pet engine exists
 *
 * Character count is wired to ZMK's keycode_state_changed event (central only).
 * All LVGL calls run on the display work queue.
 */

#include "virtual_pet.h"
#include "display_config.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <lvgl.h>

#include "icon_currency.h"
#include "pet_temp_image.h"

LOG_MODULE_REGISTER(virtual_pet, CONFIG_ZMK_LOG_LEVEL);

// ---------------------------------------------------------------------------
// Pet state
// ---------------------------------------------------------------------------
typedef enum {
    STAT_CHAR_COUNT = 0,
    STAT_HAPPINESS,
    STAT_HUNGER,
    STAT_NAME,
    STAT_COUNT,
} stat_mode_t;

static bool           pet_initialized = false;
static stat_mode_t    stat_mode       = STAT_CHAR_COUNT;
static volatile uint32_t character_count = 0;

// ---------------------------------------------------------------------------
// Widget refs (owned by custom_display.c, borrowed here after init)
// ---------------------------------------------------------------------------
static lv_obj_t *w_pet_container = NULL;
static lv_obj_t *w_status_icon   = NULL;
static lv_obj_t *w_status_label  = NULL;

// Label x positions — shifted left when no icon is shown
#define STATUS_LABEL_X_WITH_ICON  (-1 + 13 + ICON_TEXT_GAP)
#define STATUS_LABEL_X_NO_ICON    (-1)

// ---------------------------------------------------------------------------
// Status rendering (must run on display work queue)
// ---------------------------------------------------------------------------
static void render_status(void) {
    char buf[12];

    switch (stat_mode) {
        case STAT_CHAR_COUNT: {
            uint32_t n = character_count;
            if (n >= 1000000) {
                snprintf(buf, sizeof(buf), "%u.%um", (unsigned)(n / 1000000),
                         (unsigned)((n % 1000000) / 100000));
            } else if (n >= 10000) {
                snprintf(buf, sizeof(buf), "%uk", (unsigned)(n / 1000));
            } else if (n >= 1000) {
                snprintf(buf, sizeof(buf), "%u,%03u",
                         (unsigned)(n / 1000), (unsigned)(n % 1000));
            } else {
                snprintf(buf, sizeof(buf), "%u", (unsigned)n);
            }
            lv_obj_clear_flag(w_status_icon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_x(w_status_label, STATUS_LABEL_X_WITH_ICON);
            lv_label_set_text(w_status_label, buf);
            break;
        }
        case STAT_HAPPINESS:
            lv_obj_add_flag(w_status_icon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_x(w_status_label, STATUS_LABEL_X_NO_ICON);
            lv_label_set_text(w_status_label, "H: ---");
            break;
        case STAT_HUNGER:
            lv_obj_add_flag(w_status_icon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_x(w_status_label, STATUS_LABEL_X_NO_ICON);
            lv_label_set_text(w_status_label, "F: ---");
            break;
        case STAT_NAME:
            lv_obj_add_flag(w_status_icon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_x(w_status_label, STATUS_LABEL_X_NO_ICON);
            lv_label_set_text(w_status_label, "???");
            break;
        default:
            break;
    }
}

static void do_render_status(struct k_work *work) {
    if (!pet_initialized) return;
    render_status();
}
K_WORK_DEFINE(render_status_work, do_render_status);

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------
void virtual_pet_init(lv_obj_t *pet_container, lv_obj_t *status_icon,
                      lv_obj_t *status_label) {
    w_pet_container = pet_container;
    w_status_icon   = status_icon;
    w_status_label  = status_label;

    // Set initial icon source and render starting stat
    lv_img_set_src(w_status_icon, &icon_currency);
    pet_initialized = true;
    render_status();

    // Placeholder pet image — replaced when sprite system is built
    lv_obj_t *pet_img = lv_img_create(w_pet_container);
    lv_img_set_src(pet_img, &pet_temp_image);
    lv_obj_set_pos(pet_img, 0, 0);
}

void virtual_pet_cycle_stat(void) {
    stat_mode = (stat_mode_t)((stat_mode + 1) % STAT_COUNT);
    k_work_submit_to_queue(zmk_display_work_q(), &render_status_work);
}

// ---------------------------------------------------------------------------
// Character counter — central only (peripheral has no keycode events)
// ---------------------------------------------------------------------------
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>

static void do_update_char_count(struct k_work *work) {
    if (!pet_initialized) return;
    if (stat_mode == STAT_CHAR_COUNT) {
        render_status();
    }
}
K_WORK_DEFINE(update_char_count_work, do_update_char_count);

static int keycode_state_cb(const zmk_event_t *eh) {
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev && ev->state) {
        character_count++;
        k_work_submit_to_queue(zmk_display_work_q(), &update_char_count_work);
    }
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(pet_key_listener, keycode_state_cb);
ZMK_SUBSCRIPTION(pet_key_listener, zmk_keycode_state_changed);

#endif /* IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) */
