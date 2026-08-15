#!/usr/bin/env python3
"""
Convert PNG images to LVGL INDEXED_1BIT C arrays for ZMK OLED display.

─── QUICK REFERENCE ────────────────────────────────────────────────────────────

  Single image:
    python3 tools/convert_image.py resources/pet/logo.png
    → writes resources/pet/logo.h  (next to the PNG, always)

  Sprite sheet — grouped animations (RECOMMENDED for pet sprites):
    Name your sheet after the creature: keykey.png, egg.png, etc.
    Prefix defaults to the filename stem, so keykey.png → keykey_ prefix.

    python3 tools/convert_image.py resources/pet/keykey.png \\
        --sprite-w 62 --sprite-h 62 \\
        --groups idle 2 walk 3 eat 2

    Generates in keykey.h:
      keykey_idle_0, keykey_idle_1          (individual frames, 0-indexed)
      keykey_walk_0, keykey_walk_1, keykey_walk_2
      keykey_eat_0,  keykey_eat_1

      keykey_idle_frames[], keykey_idle_count    (ready-to-use in sprite_table[])
      keykey_walk_frames[], keykey_walk_count
      keykey_eat_frames[],  keykey_eat_count

  Sprite sheet — flat named frames (simple cases / backward compatible):
    python3 tools/convert_image.py resources/pet/sheet.png \\
        --sprite-w 62 --sprite-h 62 \\
        --names frame_a frame_b frame_c

  Override the prefix (when you don't want the filename as the prefix):
    python3 tools/convert_image.py resources/pet/keykey.png \\
        --sprite-w 62 --sprite-h 62 --prefix kk_ --groups idle 2 walk 3

  Write .h to a different folder:
    python3 tools/convert_image.py resources/pet/keykey.png \\
        --sprite-w 62 --sprite-h 62 --groups idle 2 walk 3 \\
        --output display_module/src/pet/

─── ALL OPTIONS ────────────────────────────────────────────────────────────────

  --groups  name count [name count ...]
            Named animation groups. count is optional and defaults to 1.
            Generates per-group frame arrays and counts.
            Prefix defaults to filename stem + underscore.

  --prefix  PREFIX
            Override the variable name prefix used with --groups.
            (Default: filename stem + '_', e.g. keykey.png → keykey_)

  --names   A B C ...
            Flat list of variable names, one per sprite. Simple cases.

  --var     NAME   Variable name for a single image (default: filename stem)
  --sprite-w N     Width of each sprite cell in pixels
  --sprite-h N     Height of each sprite cell in pixels
  --threshold N    Luminance cutoff for white (0-255, default 128)
  --invert         Flip: dark pixels → white, light pixels → black
  --output DIR     Write the .h to DIR instead of next to the source PNG
"""

import argparse
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("ERROR: Pillow not installed. Run: pip3 install Pillow", file=sys.stderr)
    sys.exit(1)


# ─── Core conversion ─────────────────────────────────────────────────────────

def image_to_lvgl_bytes(img, threshold=128, invert=False):
    """Convert a PIL image to LVGL INDEXED_1BIT byte list (palette + packed pixels)."""
    w, h = img.size
    gray = img.convert('L')
    pixels = list(gray.getdata())

    palette = [
        0x00, 0x00, 0x00, 0xFF,  # index 0: black (BGRA)
        0xFF, 0xFF, 0xFF, 0xFF,  # index 1: white (BGRA)
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
    """Format a byte list as a C array + lv_img_dsc_t struct declaration."""
    hex_vals = [f'0x{b:02X}' for b in data]
    rows = []
    for i in range(0, len(hex_vals), 16):
        rows.append('    ' + ', '.join(hex_vals[i:i + 16]) + ',')
    array_body = '\n'.join(rows)

    return (
        f'// {w}x{h} INDEXED_1BIT — {len(data)} bytes ({len(data) - 8} pixel + 8 palette)\n'
        f'static const uint8_t {var_name}_data[] = {{\n'
        f'{array_body}\n'
        f'}};\n'
        f'\n'
        f'const lv_img_dsc_t {var_name} = {{\n'
        f'    .header = {{\n'
        f'        .cf = LV_IMG_CF_INDEXED_1BIT,\n'
        f'        .always_zero = 0,\n'
        f'        .reserved = 0,\n'
        f'        .w = {w},\n'
        f'        .h = {h},\n'
        f'    }},\n'
        f'    .data_size = sizeof({var_name}_data),\n'
        f'    .data = {var_name}_data,\n'
        f'}};\n'
    )


# ─── Group name parser ───────────────────────────────────────────────────────

def parse_groups(tokens):
    """Parse 'idle 2 walk 3 eat 2' into [('idle', 2), ('walk', 3), ('eat', 2)].

    Each name can optionally be followed by a frame count (integer >= 1).
    If no count follows a name, it defaults to 1.
    Example: 'idle 2 walk 3 eat' → [('idle', 2), ('walk', 3), ('eat', 1)]
    """
    groups = []
    i = 0
    while i < len(tokens):
        name = tokens[i]
        i += 1
        count = 1
        if i < len(tokens):
            try:
                parsed = int(tokens[i])
                if parsed < 1:
                    print(f"ERROR: frame count for '{name}' must be >= 1, got {parsed}", file=sys.stderr)
                    sys.exit(1)
                count = parsed
                i += 1
            except ValueError:
                pass  # next token is another name, this group has 1 frame
        groups.append((name, count))
    return groups


# ─── Output modes ────────────────────────────────────────────────────────────

def build_single(img, args, stem, lines):
    """Single image → one lv_img_dsc_t."""
    var_name = args.var or stem
    data, w, h = image_to_lvgl_bytes(img, args.threshold, args.invert)
    lines.append(bytes_to_c_array(data, var_name, w, h))


def build_flat_sheet(img, args, stem, lines):
    """Sprite sheet with a flat list of frame names (--names)."""
    sw, sh = args.sprite_w, args.sprite_h
    iw, ih = img.size
    cols = iw // sw
    rows = ih // sh
    total = cols * rows

    frame_names = list(args.names)
    if len(frame_names) < total:
        frame_names += [f'sprite_{i}' for i in range(len(frame_names), total)]

    for idx in range(total):
        row, col = divmod(idx, cols)
        box = (col * sw, row * sh, (col + 1) * sw, (row + 1) * sh)
        data, w, h = image_to_lvgl_bytes(img.crop(box), args.threshold, args.invert)
        lines.append(bytes_to_c_array(data, frame_names[idx], w, h))

    lines.append(f'// All frames ({total} total)')
    ptrs = ', '.join(f'&{n}' for n in frame_names[:total])
    lines.append(f'static const lv_img_dsc_t *sprite_frames[] = {{ {ptrs} }};')
    lines.append(f'static const int sprite_frame_count = {total};')


def build_grouped_sheet(img, args, stem, lines):
    """Sprite sheet with named animation groups (--groups).

    Prefix defaults to filename stem + '_' so that keykey.png → keykey_ prefix,
    giving variables like keykey_idle_0, keykey_walk_1, etc.
    Override with --prefix if needed.
    """
    sw, sh = args.sprite_w, args.sprite_h
    iw, ih = img.size
    cols = iw // sw
    rows_count = ih // sh
    total_in_sheet = cols * rows_count

    prefix = args.prefix if args.prefix else (stem + '_')
    groups = parse_groups(args.groups)
    total_in_groups = sum(count for _, count in groups)

    if total_in_groups != total_in_sheet:
        print(
            f'ERROR: --groups accounts for {total_in_groups} frames '
            f'but the sheet has {total_in_sheet} '
            f'({cols} cols × {rows_count} rows at {sw}×{sh}px each).',
            file=sys.stderr,
        )
        sys.exit(1)

    lines.append(f'// Sprite sheet: {total_in_sheet} frames, prefix "{prefix}"')
    lines.append(f'// Groups: ' + ', '.join(f'{n} ({c})' for n, c in groups))
    lines.append('')

    frame_idx = 0
    group_results = []  # list of (group_name, [var_name, ...])

    for group_name, count in groups:
        var_names = []
        lines.append(f'// ── {prefix}{group_name} ({count} frame{"s" if count != 1 else ""}) ──')
        for i in range(count):
            var_name = f'{prefix}{group_name}_{i}'
            row, col = divmod(frame_idx, cols)
            box = (col * sw, row * sh, (col + 1) * sw, (row + 1) * sh)
            data, w, h = image_to_lvgl_bytes(img.crop(box), args.threshold, args.invert)
            lines.append(bytes_to_c_array(data, var_name, w, h))
            var_names.append(var_name)
            frame_idx += 1
        group_results.append((group_name, var_names))

    lines.append('// ── Frame arrays — use these in sprite_table[] in virtual_pet_render.c ──')
    lines.append('//')
    lines.append('//   [PET_BEHAVIOR_IDLE]  = ' + prefix + 'idle_frames,')
    lines.append('//   [PET_BEHAVIOR_WALK_LEFT]  = ' + prefix + 'walk_frames,')
    lines.append('//   [PET_BEHAVIOR_WALK_RIGHT] = ' + prefix + 'walk_frames,  // renderer flips it')
    lines.append('//')
    lines.append('// Also update frame_count in anim_clips[] in virtual_pet_state.c:')
    lines.append('//   [PET_BEHAVIOR_IDLE]  = { .frame_count = ' + prefix + 'idle_count, ... }')
    lines.append('')
    for group_name, var_names in group_results:
        arr  = f'{prefix}{group_name}_frames'
        cnt  = f'{prefix}{group_name}_count'
        ptrs = ', '.join(f'&{n}' for n in var_names)
        lines.append(f'static const lv_img_dsc_t *{arr}[] = {{ {ptrs} }};')
        lines.append(f'static const int {cnt} = {len(var_names)};')
        lines.append('')


# ─── Entry point ─────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument('image', help='Input PNG file')
    parser.add_argument('--threshold', type=int, default=128,
                        help='Luminance cutoff for white pixel (0-255, default 128)')
    parser.add_argument('--invert', action='store_true',
                        help='Flip: dark pixels become white, light pixels become black')
    parser.add_argument('--sprite-w', type=int, default=0,
                        help='Width of each sprite cell (enables sprite sheet mode)')
    parser.add_argument('--sprite-h', type=int, default=0,
                        help='Height of each sprite cell (enables sprite sheet mode)')
    parser.add_argument('--groups', nargs='+', default=[],
                        help='Named animation groups: name [count] name [count] ...'
                             ' (e.g. idle 2 walk 3 eat 2). Prefix = filename stem.')
    parser.add_argument('--prefix', default='',
                        help='Override variable name prefix used with --groups '
                             '(default: filename stem + underscore)')
    parser.add_argument('--names', nargs='+', default=[],
                        help='Flat list of variable names, one per sprite frame')
    parser.add_argument('--var', default='',
                        help='Variable name for a single image (default: filename stem)')
    parser.add_argument('--output', '-o', default='',
                        help='Directory to write the .h file '
                             '(default: same folder as input PNG)')
    args = parser.parse_args()

    if args.groups and args.names:
        print('ERROR: use either --groups or --names, not both.', file=sys.stderr)
        sys.exit(1)

    if args.groups and not (args.sprite_w and args.sprite_h):
        print('ERROR: --groups requires --sprite-w and --sprite-h.', file=sys.stderr)
        sys.exit(1)

    img  = Image.open(args.image)
    stem = Path(args.image).stem.lower().replace(' ', '_').replace('-', '_')

    if args.output:
        out_dir = Path(args.output)
        out_dir.mkdir(parents=True, exist_ok=True)
        out_path = out_dir / (Path(args.image).stem + '.h')
    else:
        out_path = Path(args.image).with_suffix('.h')

    lines = [
        '#pragma once',
        '#include <lvgl.h>',
        f'// Generated from {args.image} by tools/convert_image.py',
        '',
    ]

    if args.groups:
        build_grouped_sheet(img, args, stem, lines)
    elif args.sprite_w and args.sprite_h:
        build_flat_sheet(img, args, stem, lines)
    else:
        build_single(img, args, stem, lines)

    out_path.write_text('\n'.join(lines))
    print(f'Saved: {out_path}', file=sys.stderr)


if __name__ == '__main__':
    main()
