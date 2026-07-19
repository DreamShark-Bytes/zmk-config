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
- `lv_img_set_src()` with `INDEXED_1BIT` images is safe at `LV_COLOR_DEPTH=1`. The earlier hard fault was caused by undersized LVGL heap and display thread stack, not an lv_img incompatibility. Do not reduce `CONFIG_LV_Z_MEM_POOL_SIZE` (16384) or `CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE` (4096).
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

**CPU impact of display features**

Display event listeners and work items are negligible on a 64MHz Cortex-M4. All estimates below assume 5–10 key/sec typing:

| Feature | Cost per event | Duty cycle |
|---|---|---|
| Keycode event callback (char count, word boundary) | ~5 integer ops + work submit → nanoseconds | Once per keypress |
| WPM render (snprintf + lv_label_set_text) | ~10–50 µs | Once per keypress or 5s auto-cycle |
| 2s split link poll (central) | < 1ms per call | 0.05% |
| 800ms egg bob (peripheral) | lv_obj_set_y + reschedule → < 1ms | 0.1% |

The BLE radio and display backlight dominate power draw — not CPU computation. The wake-up pattern (timers preventing deep sleep between events) is the correct concern, not CPU time per event. Zephyr's tickless kernel handles short work items efficiently: the kernel sleeps between timer fires and wakes only when a timer expires.

If hard numbers are needed: enable `CONFIG_THREAD_ANALYZER=y` + `CONFIG_THREAD_ANALYZER_AUTO=y` + `CONFIG_ZMK_USB_LOGGING=y` in a test build. Type for 60 seconds while the left half is plugged in via USB and read the log for per-thread CPU% and stack high-water marks. See PLANNED.md → "Performance: BLE and Sleep Timer Baseline" for the full procedure.

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

### LVGL rendering — confirmed working

- `lv_label` with custom lv_font_conv fonts: confirmed working at `LV_COLOR_DEPTH=1`
- `lv_img_set_src()` with `INDEXED_1BIT` images: confirmed working at `LV_COLOR_DEPTH=1` once heap and stack sizes are adequate (see above)
- Canvas pixel writes (demo screen): working — pixels written directly to a raw buffer, no LVGL image pipeline involved
- All three paths are in production use in the current firmware

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
- Sizes currently built: 10, 12, 14, 16 (plus legacy 8, 9, 11 kept for reference)
- Build command: `bash tools/build_font.sh` — rebuilds all sizes; re-run after adding icons or changing sizes

**Adding a new font size — all three files must be updated:**

| Step | File | What to do |
|---|---|---|
| 1 | `tools/build_font.sh` | Add a new `lv_font_conv --size N` block and output path |
| 2 | `display_module/CMakeLists.txt` | Add `zephyr_library_sources(../resources/fonts/generated/font_badcomic_N.c)` |
| 3 | `display_module/src/display_config.h` | Add `extern const lv_font_t font_badcomic_N;` and update the `#define FONT_*` that should use it |

Run `bash tools/build_font.sh` from the project root after step 1, then push to trigger a build.
- Status icons (currency, stat indicators) are `lv_img` widgets, not inline font glyphs — simpler and avoids merging per-size font builds
- `tools/png_to_icon_font.py` is available if inline glyph icons are ever needed (e.g. scrolling notification strings), but is not currently used
- Fake bold: deferred — `build_font.sh` has a TODO stub; when implemented, `tools/apply_fake_bold.py` will post-process glyph bitmaps (1px right shift + OR) at build time, zero runtime cost

### Icon asset design

- All icons: 13×14px total with **1px bleed baked in on all sides** (12×13 white content area)
- Pet area image: 62×62px total with 1px bleed (61×61 content)
- Placement rule: to bleed off a screen edge, set the object's position to -1 toward that edge (e.g., left-edge icon at x=-1); LVGL clips the bleed pixel naturally
- Icons are `INDEXED_1BIT` (`lv_img_dsc_t`); inline status icons are embedded in the merged font as glyph bitmaps

### Display configuration

- All user-tunable constants in `display_module/src/display_config.h`:
  - Layout dimensions and split point (PET_AREA_X = 67)
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

| Half | Role | Display content |
|---|---|---|
| Left (central) | BLE central, host-connected | Custom layout: split link icon (static), BT profile + connected icon, battery %, OS icon, layer name, pet stat row |
| Right (peripheral) | BLE peripheral, split-connected | Custom layout: split link icon (live — `zmk_split_peripheral_status_changed`), battery % |

Split link icon is live on the right half only. ZMK exposes no public event for the central to detect peripheral connection status — the left link icon is static.

Pet state will track on the central and sync to the peripheral via BLE split transport (not yet designed).

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
| 2026-07-15 | Status icons as lv_img widgets, not inline font glyphs | Simpler than merging a per-glyph TTF into each font size; png_to_icon_font.py remains available for future scrolling-string use cases |
| 2026-07-15 | 1px bleed baked into all icon/pet assets | Eliminates odd pixel artifacts at screen edges; placement at x/y = -1 bleeds the pixel off-screen naturally; LVGL clips automatically |
| 2026-07-15 | Pet area 62×62 container, flush right, vertically centered | Container clips content; walking pet that moves left of x=PET_AREA_X is clipped without affecting info column |
| 2026-07-15 | Fake-bold deferred | BadComic has no bold TTF; 1px shift+OR post-process is the planned approach (build_font.sh TODO stub); deferred until layout is hardware-verified |
| 2026-07-15 | LV_LVGL_H_INCLUDE_SIMPLE required for custom_display library | lv_font_conv-generated font .c files check this symbol; ZMK defines LV_CONF_INCLUDE_SIMPLE but not this one; added via zephyr_library_compile_definitions in CMakeLists.txt |
| 2026-07-15 | Layer defines are SYMB_L/FUNC_L — not renamed to SC_L/ADJ_L | User's rename was lost in filter-repo working-tree wipe; display names set independently in display_config.h LAYER_NAMES_LIST |
| 2026-07-16 | Hard fault root cause: undersized LVGL heap and display thread stack | CONFIG_LV_Z_MEM_POOL_SIZE=16384 and CONFIG_ZMK_DISPLAY_DEDICATED_THREAD_STACK_SIZE=4096 required; lv_img with INDEXED_1BIT is safe once these are adequate |
| 2026-07-17 | All Phase 2 live data wired on both halves | Battery %, layer name, OS icon, BT profile/connected on central; battery % on peripheral; character counter live on central via keycode events |
| 2026-07-17 | Split link icon live on peripheral via zmk_split_peripheral_status_changed | Event fires on peripheral only with ev->connected; ZMK exposes no equivalent public event on the central — left link icon is static |
| 2026-07-17 | PET_AREA_X corrected 66→67 | Pet image right border (column 61) was rendering at screen x=127 (on-screen); shifting container 1px right puts it at x=128 (clipped off-screen) |
| 2026-07-17 | Font sizes updated to 10, 12, 14, 16 | Larger cap heights for better correspondence with mockup work in Clip Studio; adding a new size requires build_font.sh + CMakeLists.txt + display_config.h |
| 2026-07-18 | typing_stats.c decoupled from virtual_pet.c | Pet imports char count from typing_stats as a prerequisite; two owners of the same counter creates sync bugs and makes the pet harder to remove/replace |
| 2026-07-18 | WPM via word boundary detection (not chars/5) | Word boundary = non-alpha, non-transparent key after alpha key; more accurate than chars/5 proxy across different word length distributions |
| 2026-07-18 | Dash and apostrophe transparent for WPM | Hyphenated compounds (hello-world) and contractions (don't) count as one word each; prev_was_alpha is unchanged when either fires |
| 2026-07-18 | Modifier keycodes (0xE0–0xE7) fully transparent | Excluded from char_count and do not update word boundary state; layer keys (&mo, &lt hold) never fire zmk_keycode_state_changed and are already invisible |
| 2026-07-18 | Auto-cycle timer in custom_display.c, not typing_stats.c | Cycling is a display coordination concern; typing_stats only owns data and rendering |
| 2026-07-18 | Right half shows bobbing egg (pet_temp_image) at PET_AREA position | Placeholder that teases the virtual pet feature; link icon and battery remain in the left column of the right display |
| 2026-07-18 | Branch feature/virtual-pet after merge to main, not before | Branching before decoupling creates desync — pet branch would inherit coupled char counter; branching after gives it typing_stats.c as a ready prerequisite |
