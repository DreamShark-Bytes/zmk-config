#!/usr/bin/env python3
"""
Convert PNG images to LVGL INDEXED_1BIT C arrays for ZMK OLED display.

Usage:
  Single image:
    python3 convert_image.py image.png

  Sprite sheet (extract individual sprites):
    python3 convert_image.py sheet.png --sprite-w 16 --sprite-h 16

  Sprite sheet with named frames (for animation):
    python3 convert_image.py sheet.png --sprite-w 16 --sprite-h 16 --names idle_0 idle_1 idle_2

Output: C header file printed to stdout. Redirect to a .h file.
  python3 convert_image.py image.png > src/my_image.h

Options:
  --threshold N     Luminance threshold for white (0-255, default 128)
  --invert          Invert: dark pixels become white, light pixels black
  --sprite-w N      Width of each sprite in a sprite sheet
  --sprite-h N      Height of each sprite in a sprite sheet
  --names A B C...  Names for sprite frames (default: sprite_0, sprite_1, ...)
  --var NAME        Variable name for single image (default: from filename)
"""

import argparse
import sys
import os
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("// ERROR: Pillow not installed. Run: pip3 install Pillow", file=sys.stderr)
    sys.exit(1)


def image_to_lvgl_bytes(img, threshold=128, invert=False):
    """Convert a PIL image to LVGL INDEXED_1BIT byte list (palette + pixels)."""
    w, h = img.size
    gray = img.convert('L')
    pixels = list(gray.getdata())

    palette = [
        0x00, 0x00, 0x00, 0xFF,  # color 0: black (BGRA)
        0xFF, 0xFF, 0xFF, 0xFF,  # color 1: white (BGRA)
    ]

    data = palette[:]
    for row in range(h):
        for col in range(0, w, 8):
            byte = 0
            for bit in range(min(8, w - col)):
                lum = pixels[row * w + col + bit]
                on = (lum >= threshold) if not invert else (lum < threshold)
                if on:
                    byte |= (1 << (7 - bit))
            data.append(byte)

    return data, w, h


def bytes_to_c_array(data, var_name, w, h):
    """Format byte list as a C array + lv_img_dsc_t struct."""
    hex_vals = [f'0x{b:02X}' for b in data]
    # 16 bytes per line, aligned for readability
    lines = []
    for i in range(0, len(hex_vals), 16):
        lines.append('    ' + ', '.join(hex_vals[i:i+16]) + ',')
    array_body = '\n'.join(lines)

    return f"""\
// {w}x{h} INDEXED_1BIT — {len(data)} bytes ({len(data) - 8} pixel + 8 palette)
static const uint8_t {var_name}_data[] = {{
{array_body}
}};

const lv_img_dsc_t {var_name} = {{
    .header = {{
        .cf = LV_IMG_CF_INDEXED_1BIT,
        .always_zero = 0,
        .reserved = 0,
        .w = {w},
        .h = {h},
    }},
    .data_size = sizeof({var_name}_data),
    .data = {var_name}_data,
}};
"""


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('image', help='Input PNG file')
    parser.add_argument('--threshold', type=int, default=128,
                        help='Luminance threshold for white pixel (0-255, default 128)')
    parser.add_argument('--invert', action='store_true',
                        help='Invert: treat dark as white, light as black')
    parser.add_argument('--sprite-w', type=int, default=0,
                        help='Width of each sprite (enables sprite sheet mode)')
    parser.add_argument('--sprite-h', type=int, default=0,
                        help='Height of each sprite (enables sprite sheet mode)')
    parser.add_argument('--names', nargs='+', default=[],
                        help='Variable names for sprites (default: sprite_0, sprite_1, ...)')
    parser.add_argument('--var', default='',
                        help='Variable name for single image (default: filename stem)')
    args = parser.parse_args()

    img = Image.open(args.image)
    stem = Path(args.image).stem.lower().replace(' ', '_').replace('-', '_')

    print('#pragma once')
    print('#include <lvgl.h>')
    print(f'// Generated from {args.image} by tools/convert_image.py')
    print()

    if args.sprite_w and args.sprite_h:
        sw, sh = args.sprite_w, args.sprite_h
        iw, ih = img.size
        cols = iw // sw
        rows = ih // sh
        total = cols * rows

        frame_names = args.names or [f'sprite_{i}' for i in range(total)]
        if len(frame_names) < total:
            frame_names += [f'sprite_{i}' for i in range(len(frame_names), total)]

        index = 0
        for row in range(rows):
            for col in range(cols):
                box = (col * sw, row * sh, (col + 1) * sw, (row + 1) * sh)
                sprite = img.crop(box)
                data, w, h = image_to_lvgl_bytes(sprite, args.threshold, args.invert)
                print(bytes_to_c_array(data, frame_names[index], w, h))
                index += 1

        # Emit a const pointer array for easy animation use
        print(f'// Animation frame array ({total} frames)')
        ptrs = ', '.join(f'&{n}' for n in frame_names[:total])
        print(f'static const lv_img_dsc_t *sprite_frames[] = {{ {ptrs} }};')
        print(f'static const int sprite_frame_count = {total};')

    else:
        var_name = args.var or stem
        data, w, h = image_to_lvgl_bytes(img, args.threshold, args.invert)
        print(bytes_to_c_array(data, var_name, w, h))


if __name__ == '__main__':
    main()
