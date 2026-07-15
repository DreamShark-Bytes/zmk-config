# zmk-config — Design Document

Settled decisions and stable architecture. Updated only when a decision is finalized.

---

## Hardware

| Component | Model                              | Notes                                    |
| -----------| ------------------------------------| ------------------------------------------|
| MCU       | Nice!Nano v2 (×2)                  | nRF52840, both halves                    |
| Keyboard  | Kyria rev3                         | Split, wireless via BLE                  |
| Display   | SSD1306 OLED, 128×64px, 1-bit mono | Both halves; I²C; ~27mm × 14mm; ~120 PPI |
| Battery   | 14500 Li-ion 3.7V ~1000mAh         | One per half                             |

## Display System

### How ZMK knows the display

The display is defined in the Kyria shield's devicetree (DTS) inside the ZMK source — not in this repo. The DTS specifies the controller (`solomon,ssd1306fb`), dimensions (128×64), and I²C bus. `CONFIG_LV_COLOR_DEPTH=1` (set by the shield) tells LVGL it's 1-bit monochrome. None of this needs to be configured in zmk-config unless adding a new display type.

**PPI is not tracked by Zephyr.** Physical size must be hardcoded manually if needed (e.g., as a Kconfig constant).

### Custom display module

- Location: `display_module/` (Zephyr module, not a fork of ZMK)
- Architecture: two LVGL screens — ZMK's stock screen and a custom screen — switched with `lv_scr_load()`
- ZMK's screen is captured at first toggle press (lazy init, avoids boot timing issues)
- LVGL calls are submitted to `zmk_display_work_q()` for thread safety

### Display toggle behavior

- ZMK behavior: `&display_toggle` (`zmk,behavior-display-toggle`, 0 binding cells)
- Bound to: function layer (ADJ_L), right side row 2, slot 1
- Switches between stock ZMK display and custom display screen
- Both screens remain live; inactive screen continues updating in background

### Asset structure

| Directory | Contents |
|---|---|
| `resources/pet/` | Pet sprites — PNG source + generated `.h` (convert_image.py) |
| `resources/icons/` | UI icons — PNG source + generated `.h` (convert_image.py) |
| `resources/fonts/BadComic_Font_0_98/` | OFL font source files (committed — license permits) |
| `resources/fonts/generated/` | lv_font_conv output `.c` files — do not edit manually |
| `resources/demos/` | Generated headers from `demos/*.png` — do not edit manually |
| `demos/` | Source PNG mockups and font reference images |

### Font pipeline

- Text font: **BadComic-Regular.ttf** (SIL OFL 1.1 — may be committed to repo with attribution)
- Sizes built: 8px (layer colon), 9px (layer name), 11px (status string + inline icons), 12px (battery, BT profile, layer "L")
- Build command: `bash tools/build_font.sh` — rebuilds all sizes; re-run after adding icons or changing sizes
- Inline status icons: `tools/png_to_icon_font.py` converts a PNG to a single-glyph TTF at a Unicode private-use code point (U+E001+), then `lv_font_conv` merges it with BadComic into one font `.c` file; the icon appears inline in label strings using its UTF-8 escape sequence
- Current icon code points: `U+E001` = `icon_currency` (keycount / status prefix)
- Fake bold: deferred — `build_font.sh` has a TODO stub; when implemented, `tools/apply_fake_bold.py` will post-process glyph bitmaps (1px right shift + OR) at build time, zero runtime cost

### Icon asset design

- All icons: 13×14px total with **1px bleed baked in on all sides** (12×13 white content area)
- Pet area image: 62×62px total with 1px bleed (61×61 content)
- Placement rule: to bleed off a screen edge, set the object's position to -1 toward that edge (e.g., left-edge icon at x=-1); LVGL clips the bleed pixel naturally
- Icons are `INDEXED_1BIT` (`lv_img_dsc_t`); inline status icons are embedded in the merged font as glyph bitmaps

### Display configuration

- All user-tunable constants in `display_module/src/display_config.h`:
  - Layout dimensions and split point (PET_AREA_X = 66)
  - Row y-positions and baseline alignment offsets (tunable after hardware test)
  - Font selections (swap by changing macro, no C code changes)
  - Layer names array (via `LAYER_NAMES_LIST` macro — avoids multiple-definition issues across TUs)
  - Status string: abbreviation threshold, marquee speed, inline icon code points
- To add a new inline status icon: add PNG → run `png_to_icon_font.py` → add to `build_font.sh` → add code point macro to `display_config.h`

### Image/sprite format

- Format: LVGL `INDEXED_1BIT` (2-entry palette + 1 bit/pixel, MSB first)
- Conversion: `tools/convert_image.py` (PNG → C header, output to appropriate `resources/` subfolder)
- Sprite sheet support: `--sprite-w`, `--sprite-h`, `--names` flags; outputs `sprite_frames[]` array
- Threshold default: luminance ≥ 128 → white pixel. Override with `--threshold`
- Canvas size for pet sprites: **60×60 pixels** (fits both OLED 128×64 and Nice!View 160×68)
- Demo cycling: `tools/gen_demos.py` converts all `demos/*.png` and regenerates `resources/demos/demo_list.h`

### Dynamic display layout

- Pixel dimensions available at runtime via `lv_disp_get_hor_res(NULL)` / `lv_disp_get_ver_res(NULL)`
- Physical size/PPI not available from Zephyr; hardcode if needed: OLED ~120 PPI
- Font size presets: LVGL fonts are compiled-in C arrays at fixed sizes; "on the fly" means compiling in small/medium/large fonts and selecting via Kconfig at flash time (not true runtime scaling)

### Display split (decided)

| Half | Display content |
|---|---|
| Left (central) | Stock ZMK: BT status, battery %, layer name |
| Right (peripheral) | Custom: virtual pet |

Pet state tracks on the central and syncs to peripheral via BLE split transport (to be designed).

## Keymap

### Layer definitions

| ID | Name | Purpose |
|---|---|---|
| 0 | WINDOWS_L | Default QWERTY — Windows modifier layout |
| 1 | MAC_L | Default QWERTY — Mac modifier layout (Cmd on home row, Ctrl/Option swapped) |
| 2 | NUM_L | Number pad |
| 3 | MOV_L | Navigation / media |
| 4 | SC_L | Special characters |
| 5 | ADJ_L | Function keys, BT, OS toggle, display toggle |
| 6 | BLNDR_L | Blender shortcuts (placeholder) |
| 7 | CLPSTD_L | Clip Studio shortcuts (placeholder) |

### OS toggle

`&tog MAC_L` in ADJ_L row 2 left, slot 5 (next to BT controls). Press once to activate Mac layout; press again to return to Windows. MAC_L sits above WINDOWS_L in the layer stack and takes full priority when active. Accessible from both modes via the same `&mo NUM_L → &mo ADJ_L` path.

### Custom behaviors

| Behavior | Compatible | Description | Source |
|---|---|---|---|
| `&skq` | `zmk,behavior-sticky-key` | Sticky shift with quick-release (releases on key-down, not key-up) | Defined in `config/kyria_rev3.keymap` behaviors block |
| `&display_toggle` | `zmk,behavior-display-toggle` | Cycles through three display states: STOCK → CUSTOM → DEMO → STOCK | C driver in `display_module/src/custom_display.c`; DTS binding in `display_module/dts/bindings/zmk,behavior-display-toggle.yaml` |
| `&demo_cycle` | `zmk,behavior-demo-cycle` | Advance demo images; **no-op unless currently in DEMO state** | Thin driver in `display_module/src/demo_cycle.c`; logic in `custom_display.c` via `demo_cycle_trigger()` |

### Display states

| State | Screen shown | Activated by |
|---|---|---|
| STOCK | ZMK built-in (BT, battery, layer number) | `&display_toggle` |
| CUSTOM | Real layout: info column + pet area | `&display_toggle` |
| DEMO | Canvas showing mockup PNG images | `&display_toggle` |

`&demo_cycle` advances the demo image only when in DEMO state; pressing it in STOCK or CUSTOM is a no-op.

### Combos

| Combo | Key positions | Binding | Description |
|---|---|---|---|
| `combo_adj_l` | 24 + 45 | `&mo ADJ_L` | Both `&mo NUM_L` keys held simultaneously activates ADJ_L |

## Decision Log

| Date | Decision | Reason |
|---|---|---|
| 2026-07-07 | Nice!Nano v2 on both halves | Wireless split requires nRF52840 |
| 2026-07-07 | ZMK firmware | Only wireless-capable split firmware with Nice!Nano support |
| 2026-07-13 | Custom display as Zephyr module | Avoids forking ZMK; module system supports clean separation |
| 2026-07-13 | LVGL screen switching for toggle | Non-invasive; ZMK's widgets keep updating on their screen in the background |
| 2026-07-13 | Lazy init on first toggle press | Avoids boot timing race with zmk_display_init() running in main() after SYS_INIT |
| 2026-07-13 | 60×60 pet sprite canvas | Fits both OLED (128×64) and Nice!View (160×68) with margin |
| 2026-07-13 | Pet on right half (peripheral) | User preference; complexity of BLE state sync is a known tradeoff |
| 2026-07-13 | Font sizes as compiled-in presets | LVGL fonts are static C arrays; true runtime scaling not practical on embedded |
| 2026-07-14 | Script-based demo generation (not CMake) | CMake glob re-runs at configure time only; a one-command script is simpler and more explicit for a temporary dev tool |
| 2026-07-14 | Modifier alerts (CAPS/INSERT/NUM) dropped | Self-revealing through typing feedback; not worth screen space |
| 2026-07-14 | MockUp_4 as layout reference | Settled: left column (BT, battery, layer) + key count at bottom; pet fills right side |
| 2026-07-15 | Three-state display toggle (STOCK / CUSTOM / DEMO) | Separates real layout from dev mockup mode; users never accidentally see demo images in normal use |
| 2026-07-15 | demo_cycle is a no-op outside DEMO state | Prevents accidental image cycling while using the real layout |
| 2026-07-15 | display_config.h as single user-config file | All layout, font, and layer-name constants in one place; user never needs to touch C display code for routine customization |
| 2026-07-15 | LAYER_NAMES_LIST as a macro, not a static array in the header | Avoids multiple-definition linker errors when header is included from multiple translation units |
| 2026-07-15 | BadComic-Regular (OFL 1.1) as display font | OFL permits committing TTF to public repo with attribution; Hawtpixel Jumping (previous candidate) is donationware and cannot be distributed |
| 2026-07-15 | Inline status icons via merged lv_font_conv font | png_to_icon_font.py converts PNG → single-glyph TTF at U+E001+; lv_font_conv merges with BadComic; icon appears inline in label strings and scrolls with marquee |
| 2026-07-15 | 1px bleed baked into all icon/pet assets | Eliminates odd pixel artifacts at screen edges; placement at x/y = -1 bleeds the pixel off-screen naturally; LVGL clips automatically |
| 2026-07-15 | Pet area 62×62 container, flush right, vertically centered | Container clips content; walking pet that moves left of x=PET_AREA_X is clipped without affecting info column |
| 2026-07-15 | Fake-bold deferred | BadComic has no bold TTF; 1px shift+OR post-process is the planned approach (build_font.sh TODO stub); deferred until layout is hardware-verified |
| 2026-07-15 | Phase 2 data callbacks stubbed as commented-out functions | Real layout builds and displays placeholder data first; wiring live ZMK state is a separate step after hardware rendering is confirmed |
