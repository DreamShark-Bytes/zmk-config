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

### Hardware limitations & requirements

**nRF52840 (Nice!Nano v2)**
- Flash: 1MB total — ZMK firmware + compiled fonts + sprite assets all share this budget. Large sprite sheets or many font sizes will push against this limit.
- RAM: 256KB — the demo canvas buffer alone is 128×64 × 1 byte = 8KB. LVGL heap and Zephyr stacks compete for the rest.
- No filesystem — all assets (fonts, sprites, icons) must be compiled-in C arrays (`lv_img_dsc_t` headers). There is no FAT/LittleFS available for loading images at runtime.
- BLE split transport — central (left) ↔ peripheral (right) communicate over BLE; state sync (pet state, layer index) adds latency and must be designed for eventual consistency, not immediate consistency.

**SSD1306 OLED**
- Strictly 1-bit monochrome — no grayscale, no color. Every pixel is on or off.
- `LV_COLOR_DEPTH=1` is set by the Kyria shield DTS inside ZMK source — cannot be changed here without modifying upstream ZMK. All LVGL rendering must work within this constraint.
- Fixed 128×64 pixel canvas — layout is pixel-budget design, not responsive.

**ZMK / LVGL**
- ZMK version: **v0.3** (pinned in `config/west.yml`). Zephyr and LVGL versions are determined by ZMK's own `app/west.yml` — not independently controlled here.
- All LVGL calls must be made from the display work queue (`zmk_display_work_q()`) — calling LVGL from any other thread is unsafe.
- **Known incompatibility:** `lv_img_set_src()` with `INDEXED_1BIT` images crashes at `LV_COLOR_DEPTH=1` (hard fault, split disconnects). Canvas pixel writes work. Status: under investigation — adding layout elements incrementally to isolate.
- Font sizes are compiled-in static C arrays — no runtime font scaling. Adding a new size requires rebuilding via `tools/build_font.sh`.

### Resource estimates

Exact numbers come from the Zephyr memory report in the GitHub Actions build log (see CLAUDE.md → "Build & flash workflow"). Last measured: 2026-07-16, commit `7aac4e0`.

**Flash**

Available region: **792 KB** (1MB nRF52840 total, minus bootloader + NVS partitions)

| | Used | Available | % used |
|---|---|---|---|
| Flash | 329 KB | 792 KB | 40.6% |
| RAM | 65 KB | 256 KB | 25.5% |

Headroom: ~463 KB flash, ~191 KB RAM. Pet sprite sheets will be the next meaningful addition — a multi-frame animation at 60×60px 1-bit runs ~450 bytes per frame. 20 frames ≈ 9 KB. Nowhere near the limit.

Note: the 792 KB flash region (not 1024 KB) reflects partitioning — the bootloader, MBR, and NVS settings storage claim the remainder of the 1MB chip before ZMK's app partition begins.

**RAM**

The largest fixed allocation from our code is the demo canvas buffer: 128 × 64 × 1 byte = **8 KB**. LVGL heap (ZMK default ~8–16 KB), Zephyr stacks, and ZMK core account for the rest. RAM is not a current concern.

**Power consumption**

The SSD1306 OLED dominates power draw — not the MCU.

| State | Estimated draw |
|---|---|
| nRF52840 BLE connected, active | ~3–8 mA |
| SSD1306 OLED on | +8–15 mA |
| SSD1306 OLED blanked/sleeping | +1–2 mA |
| Both displays on, typing | ~15–25 mA |
| Idle, displays off | ~3–5 mA |

With 14500 1000mAh batteries:
- Display always on: ~50 hours
- Display blanked on idle (ZMK default): ~150–250 hours

**Key config levers for battery life:**
- `CONFIG_ZMK_DISPLAY_BLANK_ON_IDLE` — blanks display after inactivity timeout
- `CONFIG_ZMK_IDLE_SLEEP_TIMEOUT` — deep sleep after longer inactivity (~100µA draw)

These are already present in `config/kyria_rev3.conf`. Tuning their timeout values is the primary tool for extending battery life.

## Display System

### How ZMK knows the display

The display is defined in the Kyria shield's devicetree (DTS) inside the ZMK source — not in this repo. The DTS specifies the controller (`solomon,ssd1306fb`), dimensions (128×64), and I²C bus. `CONFIG_LV_COLOR_DEPTH=1` (set by the shield) tells LVGL it's 1-bit monochrome. None of this needs to be configured in zmk-config unless adding a new display type.

**PPI is not tracked by Zephyr.** Physical size must be hardcoded manually if needed (e.g., as a Kconfig constant).

### Custom display module

- Location: `display_module/` (Zephyr module, not a fork of ZMK)
- Architecture: three LVGL screens — ZMK stock, real layout (CUSTOM), demo canvas (DEMO) — switched with `lv_scr_load()`
- ZMK's screen is captured at first toggle press (lazy init, avoids boot timing issues)
- LVGL calls are submitted to `zmk_display_work_q()` for thread safety
- `LV_LVGL_H_INCLUDE_SIMPLE` must be defined for the custom_display library (via `zephyr_library_compile_definitions`) — lv_font_conv-generated font `.c` files check this symbol to select between `"lvgl.h"` and `"lvgl/lvgl.h"`; ZMK only provides the former

### LVGL rendering — known constraint

- The demo screen (canvas-based) works at `LV_COLOR_DEPTH=1`: pixels are written directly to a raw buffer, bypassing the LVGL image pipeline
- `lv_img_set_src()` with `INDEXED_1BIT` images at `LV_COLOR_DEPTH=1` causes a hard fault on hardware — root cause not yet isolated. Symptoms: split halves disconnect, a layer gets stuck active, requires power cycle
- `lv_label` with custom fonts has not yet been tested at `LV_COLOR_DEPTH=1`
- Strategy: add layout elements back to `build_real_screen()` one at a time (label first, then image) to isolate which LVGL path crashes. Canvas-based rendering is the fallback if `lv_img` is incompatible

### Display toggle behavior

- ZMK behavior: `&display_toggle` (`zmk,behavior-display-toggle`, 0 binding cells)
- Cycles through: STOCK → CUSTOM → DEMO → STOCK
- Confirmed working on hardware (2026-07-15)
- `&demo_cycle` is a no-op in STOCK and CUSTOM; only advances demo images in DEMO state

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
| 4 | SYMB_L | Special characters |
| 5 | FUNC_L | Function keys, BT, OS toggle, display toggle |
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
| 2026-07-15 | LV_LVGL_H_INCLUDE_SIMPLE required for custom_display library | lv_font_conv-generated font .c files check this symbol; ZMK defines LV_CONF_INCLUDE_SIMPLE but not this one; added via zephyr_library_compile_definitions in CMakeLists.txt |
| 2026-07-15 | lv_img with INDEXED_1BIT crashes at LV_COLOR_DEPTH=1 | Hard fault on hardware when build_real_screen() used lv_img_set_src; canvas-based approach (demo screen) works; isolating crash incrementally next session |
| 2026-07-15 | Real layout stubbed as empty screen until lv_img crash is resolved | Layout code preserved as comments in custom_display.c; three-state toggle confirmed working on hardware with stub |
| 2026-07-15 | Layer defines are SYMB_L/FUNC_L — not renamed to SC_L/ADJ_L | User's rename was lost in filter-repo working-tree wipe; display names set independently in display_config.h LAYER_NAMES_LIST |
