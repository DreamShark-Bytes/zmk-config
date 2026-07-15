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

- **`&display_toggle`** — switches between the stock ZMK display and the custom screen. Bound to ADJ_L.
- **`&demo_cycle`** — steps through demo/mockup images on the custom screen. Bound to ADJ_L next to `&display_toggle`. Wraps after the last image.

### Display layout

| Half | Display content |
|---|---|
| Left | Stock ZMK: Bluetooth status + profile, battery %, layer indicator |
| Right | Custom: virtual pet (in development) |

### Asset structure

```
resources/
  pet/      ← 60×60 pet sprites (.png source + generated .h)
  icons/    ← small UI icons (.png source + generated .h)
  fonts/    ← TTF/OTF source files + generated LVGL .c files + license files
  demos/    ← generated headers from demos/*.png (do not edit manually)
demos/      ← source PNG mockups and font reference images
```

### Adding images and sprites

Use `tools/convert_image.py` to convert a PNG to an LVGL C header:

```bash
# Single image — output to the appropriate resources/ subfolder
python3 tools/convert_image.py image.png > resources/pet/my_image.h

# Sprite sheet (e.g. 4 frames, 60×60 each)
python3 tools/convert_image.py sheet.png --sprite-w 60 --sprite-h 60 \
  --names idle_0 idle_1 walk_0 walk_1 > resources/pet/sprites.h
```

Images must be PNG. The display is **monochrome** — pixels with luminance ≥ 128 render as white (lit), below 128 as black (off). Grey and color pixels are converted automatically.

### Demo image cycling

To add a mockup or test image to the on-device cycle:

1. Drop the PNG in `demos/`
2. Run `python3 tools/gen_demos.py` from the project root
3. Commit the generated files in `resources/demos/` before building

`gen_demos.py` converts every PNG in `demos/` and regenerates `resources/demos/demo_list.h` with the full image array.

### Building locally

The module path in `build.yaml` is hardcoded to this machine. To build with GitHub Actions CI, the module needs to be extracted to a separate GitHub repo and referenced in `config/west.yml`.

## Architecture

See [DESIGN.md](DESIGN.md) for settled decisions and technical architecture.
See [PLANNED.md](PLANNED.md) for features in progress.
