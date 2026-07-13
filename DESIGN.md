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

### Image/sprite format

- Format: LVGL `INDEXED_1BIT` (2-entry palette + 1 bit/pixel, MSB first)
- Conversion: `tools/convert_image.py` (PNG → C header)
- Sprite sheet support: `--sprite-w`, `--sprite-h`, `--names` flags; outputs `sprite_frames[]` array
- Threshold default: luminance ≥ 128 → white pixel. Override with `--threshold`
- Canvas size for pet sprites: **60×60 pixels** (fits both OLED 128×64 and Nice!View 160×68)

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

| Behavior | Compatible | Description |
|---|---|---|
| `&skq` | `zmk,behavior-sticky-key` | Sticky shift with quick-release (releases on key-down, not key-up) |
| `&display_toggle` | `zmk,behavior-display-toggle` | Toggle custom display on/off |

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
