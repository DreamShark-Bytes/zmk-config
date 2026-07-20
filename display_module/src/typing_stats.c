/*
 * typing_stats.c — character count and rolling-window WPM display.
 *
 * Stat modes (cycled by &cycle in CUSTOM display state, or auto-cycled by timer):
 *   STAT_WPM         — rolling-window WPM; shows "--" when no words in the window
 *   STAT_CHAR_COUNT  — keypresses this session
 *
 * WPM uses a configurable rolling window (STAT_WPM_WINDOW_MS in display_config.h).
 * When no words have been typed in the last STAT_WPM_WINDOW_MS milliseconds the
 * display shows "--", so idle time never dilutes the reading.
 *
 * Word boundary detection: a word completes when a non-alpha, non-transparent key
 * follows an alpha key. Dash and apostrophe are transparent (hyphenated compounds
 * and contractions count as one word each). Modifier keys (Shift, Ctrl, Alt, Meta)
 * are fully transparent — not counted in char_count and invisible to word state.
 * Layer keys (&mo, &lt hold, &to) never fire zmk_keycode_state_changed and are
 * therefore already invisible to this module.
 *
 * Privacy: no keystrokes or text content are recorded. The ring buffer stores only
 * timestamps (ms since boot) of when word boundaries were detected — the keyboard
 * cannot reconstruct what was typed from this data.
 *
 * Character counting is wired to ZMK's keycode_state_changed event (central only).
 * All LVGL calls run on the display work queue.
 *
 * The virtual pet reads char_count as a prerequisite — this module owns the counter.
 */

#include "typing_stats.h"
#include "display_config.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <lvgl.h>

#include "icon_currency.h"

LOG_MODULE_REGISTER(typing_stats, CONFIG_ZMK_LOG_LEVEL);

/* HID Keyboard/Keypad usage page (USB HID spec §10) */
#define HID_USAGE_PAGE_KEY   0x07u

/* HID keyboard usage IDs (all standard, will not change) */
#define HID_KEY_A            0x04u   /* a / A — first alpha key */
#define HID_KEY_Z            0x1Du   /* z / Z — last alpha key  */
#define HID_KEY_MINUS        0x2Du   /* - / _  transparent for hyphenated words */
#define HID_KEY_APOSTROPHE   0x34u   /* ' / "  transparent for contractions     */
#define HID_KEY_MOD_FIRST    0xE0u   /* Left Control — first modifier keycode   */
#define HID_KEY_MOD_LAST     0xE7u   /* Right GUI    — last modifier keycode    */

typedef enum {
    STAT_WPM = 0,
    STAT_CHAR_COUNT,
    STAT_COUNT,
} stat_mode_t;

/* Ring buffer capacity: sized for 240 WPM over the window (safe ceiling). */
#define ROLLING_WORD_CAPACITY 120u

static bool        stats_initialized = false;
static stat_mode_t stat_mode         = STAT_WPM;
static uint32_t    char_count        = 0;
static bool        prev_was_alpha    = false;

/* word_timestamps: ms-since-boot of each word completion (ring buffer). */
static int64_t  word_timestamps[ROLLING_WORD_CAPACITY];
static uint32_t word_buf_count = 0;   /* total words ever typed */

static lv_obj_t *w_status_icon  = NULL;
static lv_obj_t *w_status_label = NULL;

#define STATUS_LABEL_X_NO_ICON    (-1)

static void render_stat(void) {
    char buf[12];

    switch (stat_mode) {
        case STAT_CHAR_COUNT: {
            uint32_t n = char_count;
            if (n >= 1000000000u) {
                uint32_t w = n / 1000000000u, r = n % 1000000000u;
                if      (w >= 100) snprintf(buf, sizeof(buf), "c %u.%ub",   w, r / 100000000u);
                else if (w >= 10)  snprintf(buf, sizeof(buf), "c %u.%02ub", w, r / 10000000u);
                else               snprintf(buf, sizeof(buf), "c %u.%03ub", w, r / 1000000u);
            } else if (n >= 1000000) {
                uint32_t w = n / 1000000, r = n % 1000000;
                if      (w >= 100) snprintf(buf, sizeof(buf), "c %u.%um",   w, r / 100000);
                else if (w >= 10)  snprintf(buf, sizeof(buf), "c %u.%02um", w, r / 10000);
                else               snprintf(buf, sizeof(buf), "c %u.%03um", w, r / 1000);
            } else if (n >= 100000) {
                snprintf(buf, sizeof(buf), "c %u.%uk", n / 1000, (n % 1000) / 100);
            } else if (n >= 1000) {
                snprintf(buf, sizeof(buf), "c %u,%03u", n / 1000, n % 1000);
            } else {
                snprintf(buf, sizeof(buf), "c %u", (unsigned)n);
            }
            lv_obj_add_flag(w_status_icon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_x(w_status_label, STATUS_LABEL_X_NO_ICON);
            lv_label_set_text(w_status_label, buf);
            break;
        }
        case STAT_WPM: {
            lv_obj_add_flag(w_status_icon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_x(w_status_label, STATUS_LABEL_X_NO_ICON);

            int64_t now_ms    = k_uptime_get();
            /* Shrink window to elapsed time during warmup so WPM isn't over-inflated
             * when fewer than STAT_WPM_WINDOW_MS ms have passed since boot. */
            int64_t window_ms = (now_ms < (int64_t)STAT_WPM_WINDOW_MS)
                                ? now_ms : (int64_t)STAT_WPM_WINDOW_MS;
            int64_t cutoff    = now_ms - window_ms;

            uint32_t valid = (word_buf_count < ROLLING_WORD_CAPACITY)
                             ? word_buf_count : ROLLING_WORD_CAPACITY;
            uint32_t count = 0;
            for (uint32_t i = 0; i < valid; i++) {
                if (word_timestamps[i] >= cutoff) count++;
            }

            if (count == 0 || window_ms < 5000) {
                lv_label_set_text(w_status_label, "WPM --");
            } else {
                uint32_t wpm = (uint32_t)((count * 60000LL) / window_ms);
                snprintf(buf, sizeof(buf), "WPM %u", (unsigned)wpm);
                lv_label_set_text(w_status_label, buf);
            }
            break;
        }
        default:
            break;
    }
}

static void do_render_stat(struct k_work *work) {
    if (!stats_initialized) return;
    render_stat();
}
K_WORK_DEFINE(render_stat_work, do_render_stat);

void typing_stats_init(lv_obj_t *status_icon, lv_obj_t *status_label) {
    w_status_icon  = status_icon;
    w_status_label = status_label;
    lv_img_set_src(w_status_icon, &icon_currency);
    stats_initialized = true;
    render_stat();
}

void typing_stats_cycle(void) {
    stat_mode = (stat_mode_t)((stat_mode + 1) % STAT_COUNT);
    k_work_submit_to_queue(zmk_display_work_q(), &render_stat_work);
}

// ---------------------------------------------------------------------------
// Character counter and word boundary detector — central only
// ---------------------------------------------------------------------------
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>

static void do_update_stat(struct k_work *work) {
    if (!stats_initialized) return;
    render_stat();
}
K_WORK_DEFINE(update_stat_work, do_update_stat);

static int keycode_state_cb(const zmk_event_t *eh) {
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (!ev || !ev->state) return ZMK_EV_EVENT_BUBBLE;

    /* Only process keyboard usage page events */
    if (ev->usage_page != HID_USAGE_PAGE_KEY) return ZMK_EV_EVENT_BUBBLE;

    uint32_t kc = ev->keycode;

    /* Modifier keys are fully transparent — don't count, don't affect word state */
    if (kc >= HID_KEY_MOD_FIRST && kc <= HID_KEY_MOD_LAST) return ZMK_EV_EVENT_BUBBLE;

    /* Character count: all non-modifier keyboard keypresses */
    char_count++;

    /* Word boundary detection */
    bool is_alpha       = (kc >= HID_KEY_A && kc <= HID_KEY_Z);
    bool is_transparent = (kc == HID_KEY_MINUS || kc == HID_KEY_APOSTROPHE);

    if (!is_alpha && !is_transparent && prev_was_alpha) {
        word_timestamps[word_buf_count % ROLLING_WORD_CAPACITY] = k_uptime_get();
        word_buf_count++;
    }
    /* Dash and apostrophe leave prev_was_alpha unchanged — hyphenated compounds
     * and contractions remain a single word in progress */
    if (!is_transparent) {
        prev_was_alpha = is_alpha;
    }

    k_work_submit_to_queue(zmk_display_work_q(), &update_stat_work);
    return ZMK_EV_EVENT_BUBBLE;
}
ZMK_LISTENER(stats_key_listener, keycode_state_cb);
ZMK_SUBSCRIPTION(stats_key_listener, zmk_keycode_state_changed);

#endif /* IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL) */
