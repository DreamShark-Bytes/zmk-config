#pragma once

// =============================================================================
// display_config.h — Custom display configuration
//
// Edit this file to customize layout, fonts, and behavior.
// After editing font sizes: run tools/build_font.sh, then push to GitHub.
// All other changes: push to GitHub to trigger a firmware rebuild.
// =============================================================================


// -----------------------------------------------------------------------------
// DISPLAY DIMENSIONS — SSD1306 OLED (do not change for Kyria rev3)
// -----------------------------------------------------------------------------
#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 64


// -----------------------------------------------------------------------------
// LAYER NAMES
// Index matches the layer number in kyria_rev3.keymap.
// NULL entries display as the raw layer number automatically.
// Max readable length at font_badcomic_12 is ~5 characters before CLIP truncates.
// To add more layers: increase LAYER_NAME_COUNT and extend LAYER_NAMES_LIST.
//
// LAYER_IDX_MAC: which layer index activates the Mac OS icon.
// Layer 0 = Windows (default), layer 1 = Mac. Update if you reorder layers.
// -----------------------------------------------------------------------------
#define LAYER_IDX_MAC 1
#define LAYER_NAME_COUNT 8

#define LAYER_NAMES_LIST \
    "BASE",  /* 0  WINDOWS_L */ \
    "BASE",  /* 1  MAC_L     */ \
    "NUM",   /* 2  NUM_L     */ \
    "MOVE",  /* 3  MOV_L     */ \
    "SYMB",  /* 4  SC_L      */ \
    "FUNC",  /* 5  ADJ_L     */ \
    NULL,    /* 6  unused    */ \
    NULL,    /* 7  unused    */


// -----------------------------------------------------------------------------
// FONT SELECTIONS
// Swap a font by changing the name and running tools/build_font.sh.
// Available sizes: font_badcomic_8, font_badcomic_9, font_badcomic_10,
//                  font_badcomic_11, font_badcomic_12, font_badcomic_14, font_badcomic_16
// To use your own font: add its .c to resources/fonts/generated/ and
// list it in display_module/CMakeLists.txt.
// -----------------------------------------------------------------------------
extern const lv_font_t font_badcomic_10;
extern const lv_font_t font_badcomic_12;
extern const lv_font_t font_badcomic_14;
extern const lv_font_t font_badcomic_16;

#define FONT_LAYER_COLON  (&font_badcomic_10)    // ":" between "L" and name
#define FONT_LAYER_NAME   (&font_badcomic_12)    // layer name or number
#define FONT_STATUS_TEXT  (&font_badcomic_14)    // bottom status string (WPM / char count)
#define FONT_BATTERY_NUM  (&font_badcomic_16)    // battery percentage digits
#define FONT_LAYER_L      (&font_badcomic_16)    // "L" label
#define FONT_BT_PROFILE   (&font_badcomic_16)    // BT profile number


// -----------------------------------------------------------------------------
// ANIMATION & TIMING
//
// EGG_BOB_INTERVAL_MS: how fast the right-half egg alternates between its two
//   animation frames. Lower = faster animation.
//
// STAT_CYCLE_INTERVAL_MS: how long each typing stat is shown before the display
//   auto-advances to the next. Set to 0 to disable auto-cycle (&cycle key only).
//
// STAT_WPM_WINDOW_MS: rolling time window used to calculate WPM. Only words
//   typed within this window count. Idle time falls out naturally, so it never
//   drags the reading down. 30 000 ms = 30 seconds.
// -----------------------------------------------------------------------------
#define EGG_BOB_INTERVAL_MS      800   // ms per animation frame (lower = faster)
#define STAT_CYCLE_INTERVAL_MS  5000   // ms between auto-cycle; 0 = &cycle key only
#define STAT_WPM_WINDOW_MS     30000   // rolling WPM window in ms


// -----------------------------------------------------------------------------
// LAYOUT — PET AREA
// The screen is divided into a left info column and a right pet area.
// PET_AREA_X is the left edge of the pet container. Its right edge extends 1px
// off-screen (bleed pixel disappears at the display boundary).
// The info column occupies x=0 to (PET_AREA_X - 1).
// -----------------------------------------------------------------------------
#define PET_AREA_X       67   // left edge of pet container
#define PET_AREA_WIDTH   62   // total width including 1px bleed each side
#define PET_AREA_HEIGHT  62   // total height including 1px bleed each side
#define PET_AREA_Y        1   // top y: (DISPLAY_HEIGHT - PET_AREA_HEIGHT) / 2


// -----------------------------------------------------------------------------
// LAYOUT — INFO COLUMN ROW POSITIONS
// y coordinate of the top of each row's bounding box.
// Icons are 13×14px total (12×13 white content + 1px bleed).
// Top-row icon bleeds 1px off the top edge, so its y = -1.
// -----------------------------------------------------------------------------
#define ROW_TOP_Y      (-1)   // link + BT row: 1px bleed off top edge
#define ROW_BATTERY_Y   14    // battery/lightning icon + percentage
#define ROW_LAYER_Y     30    // OS icon + "L:" + layer name


// -----------------------------------------------------------------------------
// LAYOUT — LABEL VERTICAL OFFSETS
// Icons are 14px total: 1px top bleed + 12px content + 1px bottom bleed.
// LARGE_FONT_Y_OFFSET: shifts font_14/16 labels up so their cap height (≈12px)
//   aligns with the icon content window. Applied to BT profile, battery %,
//   layer "L", and status text.
// LAYER_COLON_Y_OFFSET: bottom-justifies ":" (font_10, cap≈8px) in the row.
// LAYER_NAME_Y_OFFSET: baselines the font_12 layer name with the font_16 "L".
// -----------------------------------------------------------------------------
#define LARGE_FONT_Y_OFFSET   (-2) // cap-align font_14/16 labels to icon content
#define LAYER_COLON_Y_OFFSET    3  // y offset for ":" relative to ROW_LAYER_Y
#define LAYER_NAME_Y_OFFSET     0  // y offset for layer name relative to ROW_LAYER_Y


// -----------------------------------------------------------------------------
// LAYOUT — ICON AND TEXT SPACING
// All left-column icons have their x at -1 (1px left bleed off-screen).
// ICON_TEXT_GAP: pixels between the icon's right content edge (at x=12) and
// the next element.
// -----------------------------------------------------------------------------
#define ICON_TEXT_GAP    2    // gap between icon right edge and next item (px)
#define ICON_CONTENT_W  12   // white pixel width of icons (13 total - 1 bleed)
