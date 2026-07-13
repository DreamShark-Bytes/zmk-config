# zmk-config — Kyria Split Keyboard

Personal ZMK firmware config for the [Kyria rev3](https://splitkb.com/kyria) wireless split keyboard, running on Nice!Nano v2 microcontrollers.

ZMK is required for wireless split keyboard support on the Nice!Nano hardware.

## Planned Uses

- **Virtual pet** — a Tamagotchi-style creature living on the OLED displays, fed by keypresses
- **Drawing tablet companion** — one half used as a keypad alongside a Wacom tablet or iPad, keeping shortcuts accessible without a full keyboard on the desk

## Keyboard Layout

![Kyria keyboard mapping](kyria_keyboard_mapping.png)

## Custom Display Module

The firmware includes a custom Zephyr module (`display_module/`) that extends the OLED displays beyond ZMK's default widgets.

### Current features

- **`&display_toggle`** — a custom ZMK behavior that switches between the stock display (battery, Bluetooth status, layer) and a custom display screen. Bound to the function layer (ADJ_L).

### Display layout

| Half | Display content |
|---|---|
| Left | Stock ZMK: Bluetooth status + profile, battery %, layer indicator |
| Right | Custom: virtual pet (in development) |

### Adding images and sprites

Use `tools/convert_image.py` to convert PNG files to LVGL C arrays:

```bash
# Single image
python3 tools/convert_image.py image.png > display_module/src/my_image.h

# Sprite sheet (e.g. 4 frames, 60×60 each)
python3 tools/convert_image.py sheet.png --sprite-w 60 --sprite-h 60 \
  --names idle_0 idle_1 walk_0 walk_1 > display_module/src/sprites.h
```

Images must be PNG. The display is **monochrome** — pixels with luminance ≥ 128 render as white (lit), below 128 as black (off). Grey and color pixels are converted automatically.

### Building locally

The module path in `build.yaml` is hardcoded to this machine. To build with GitHub Actions CI, the module needs to be extracted to a separate GitHub repo and referenced in `config/west.yml`.

## Architecture

See [DESIGN.md](DESIGN.md) for settled decisions and technical architecture.
See [PLANNED.md](PLANNED.md) for features in progress.
