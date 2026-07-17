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
// LAYOUT SPLIT
// The screen is divided into a left info column and a right pet area.
// PET_AREA_X is the left edge of the 62px pet container. Its right edge
// extends 1px off-screen (bleed pixel disappears at the display boundary).
// The info column occupies x=0 to (PET_AREA_X - 1).
// -----------------------------------------------------------------------------
#define PET_AREA_X       66   // left edge of pet container
#define PET_AREA_WIDTH   62   // total width including 1px bleed each side
#define PET_AREA_HEIGHT  62   // total height including 1px bleed each side
#define PET_AREA_Y        1   // top y: (DISPLAY_HEIGHT - PET_AREA_HEIGHT) / 2


// -----------------------------------------------------------------------------
// INFO COLUMN — ROW Y POSITIONS
// y coordinate of the top of each row's bounding box.
// Icons are 13×14px total (12×13 white content + 1px bleed).
// Top-row icon bleeds 1px off the top: its y = -1.
// Status text is bottom-anchored at runtime.
// -----------------------------------------------------------------------------
#define ROW_TOP_Y      (-1)   // link + BT row: 1px bleed off top edge
#define ROW_BATTERY_Y   14    // battery/lightning icon + percentage
#define ROW_LAYER_Y     30    // OS icon + "L:" + layer name


// -----------------------------------------------------------------------------
// LAYER ROW — VERTICAL OFFSETS FOR BASELINE ALIGNMENT
// "L" (size 12) sets the baseline. ":" and name are nudged down so their
// baselines align with "L". Tweak these after hardware testing.
//   font_badcomic_12: line_height=16, base_line=3  → ascent=13
//   font_badcomic_8:  line_height=9,  base_line=2  → ascent=7
//   font_badcomic_9:  line_height=11, base_line=2  → ascent=9
// -----------------------------------------------------------------------------
#define LAYER_COLON_Y_OFFSET  6   // y += this for ":" relative to ROW_LAYER_Y
#define LAYER_NAME_Y_OFFSET   4   // y += this for layer name relative to ROW_LAYER_Y


// -----------------------------------------------------------------------------
// INFO COLUMN — ICON AND TEXT SPACING
// All left-column icons have their x at -1 (1px left bleed off-screen).
// ICON_TEXT_GAP: pixels between the icon's right edge (at x=12) and the
// next element.
// -----------------------------------------------------------------------------
#define ICON_TEXT_GAP    2    // gap between icon right content edge and next item
#define ICON_CONTENT_W  12   // white pixel width of icons (13 total - 1 bleed)


// -----------------------------------------------------------------------------
// LAYER NAMES
// Index matches the layer number in kyria_rev3.keymap.
// NULL entries display as the raw layer number automatically.
// Max readable length at font size 9 is ~5 characters before CLIP truncates.
// To add more layers: increase LAYER_NAME_COUNT and extend LAYER_NAMES_LIST.
// -----------------------------------------------------------------------------
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
// Available sizes: font_badcomic_8, font_badcomic_9,
//                  font_badcomic_11, font_badcomic_12
// To use your own font: add its .c to resources/fonts/generated/ and
// list it in display_module/CMakeLists.txt.
// -----------------------------------------------------------------------------
extern const lv_font_t font_badcomic_8;
extern const lv_font_t font_badcomic_9;
extern const lv_font_t font_badcomic_11;
extern const lv_font_t font_badcomic_12;

#define FONT_LAYER_COLON  (&font_badcomic_8)    // ":" between "L" and name
#define FONT_LAYER_NAME   (&font_badcomic_9)    // layer name or number
#define FONT_STATUS_TEXT  (&font_badcomic_11)   // bottom status string
#define FONT_BATTERY_NUM  (&font_badcomic_12)   // battery percentage digits
#define FONT_LAYER_L      (&font_badcomic_12)   // "L" label
#define FONT_BT_PROFILE   (&font_badcomic_12)   // BT profile number
