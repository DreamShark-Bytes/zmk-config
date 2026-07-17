#!/usr/bin/env python3
"""
tools/png_to_icon_font.py

Converts a 1-bit PNG icon to a minimal TTF font, assigning the icon as a glyph
at a Unicode private-use code point. The output TTF is then merged with BadComic
(or any text font) via lv_font_conv to produce a single LVGL font .c file where
the icon can appear inline in label strings.

Usage:
  python3 tools/png_to_icon_font.py <icon.png> <output.ttf> [--codepoint 0xE001]

Example (currency icon at U+E001, dark-on-transparent design):
  python3 tools/png_to_icon_font.py resources/icons/icon_currency.png \\
      /tmp/icon_currency.ttf --codepoint 0xE001 --invert

Then merge with text font via lv_font_conv — see tools/build_font.sh.
"""

import argparse
import sys
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("Pillow not installed. Run: pip3 install Pillow", file=sys.stderr)
    sys.exit(1)

try:
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
except ImportError:
    print("fonttools not installed. Run: pip3 install fonttools", file=sys.stderr)
    sys.exit(1)


def png_to_icon_ttf(png_path, output_path, codepoint, invert=False):
    img = Image.open(png_path).convert("RGBA")
    width, height = img.size
    pixels = img.load()

    # Font units per pixel. Large enough for good rasterization precision.
    SCALE = 64
    upm = max(width, height) * SCALE
    ascent = height * SCALE

    glyph_name = Path(png_path).stem.replace("-", "_").replace(" ", "_")

    fb = FontBuilder(upm, isTTF=True)
    fb.setupGlyphOrder([".notdef", glyph_name])
    fb.setupCharacterMap({codepoint: glyph_name})

    # .notdef: empty box outline
    nd_pen = TTGlyphPen(None)
    nd_pen.moveTo((0, 0))
    nd_pen.lineTo((width * SCALE, 0))
    nd_pen.lineTo((width * SCALE, height * SCALE))
    nd_pen.lineTo((0, height * SCALE))
    nd_pen.closePath()

    # Icon glyph: one filled square per selected pixel.
    # --invert: dark pixels (icon drawn dark-on-transparent) → glyph.
    # default:  light pixels (icon drawn white-on-transparent) → glyph.
    # CCW contours in font space (y-up) = filled outer contour.
    icon_pen = TTGlyphPen(None)
    pixel_count = 0
    for py in range(height):
        for px in range(width):
            r, g, b, a = pixels[px, py]
            bright = (r + g + b) > 383
            if a > 128 and (bright != invert):
                pixel_count += 1
                # Flip y: PNG y=0 is top; TTF y=0 is bottom
                fy = height - 1 - py
                x0, y0 = px * SCALE, fy * SCALE
                x1, y1 = x0 + SCALE, y0 + SCALE
                # CCW in y-up: BL → BR → TR → TL
                icon_pen.moveTo((x0, y0))
                icon_pen.lineTo((x1, y0))
                icon_pen.lineTo((x1, y1))
                icon_pen.lineTo((x0, y1))
                icon_pen.closePath()

    if pixel_count == 0:
        print(f"Warning: no white/opaque pixels found in {png_path}", file=sys.stderr)

    fb.setupGlyf({".notdef": nd_pen.glyph(), glyph_name: icon_pen.glyph()})
    fb.setupHorizontalMetrics({
        ".notdef": (upm, 0),
        glyph_name: (width * SCALE, 0),
    })
    fb.setupHorizontalHeader(ascent=ascent, descent=0)
    fb.setupNameTable({"familyName": "IconFont", "styleName": "Regular"})
    fb.setupOS2(
        sTypoAscender=ascent, sTypoDescender=0, sTypoLineGap=0,
        usWinAscent=ascent, usWinDescent=0,
    )
    fb.setupPost()
    fb.font.save(output_path)
    mode = "dark" if invert else "light"
    print(f"Saved: {output_path}  ({pixel_count} {mode} pixels → glyph U+{codepoint:04X})")


def main():
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("png", help="Input PNG icon file")
    parser.add_argument("output", help="Output TTF file path")
    parser.add_argument(
        "--codepoint", default="0xE001",
        help="Unicode private-use code point (default: 0xE001)",
    )
    parser.add_argument(
        "--invert", action="store_true",
        help="Select dark pixels as the glyph (for icons drawn dark-on-transparent, matching convert_image.py --invert convention)",
    )
    args = parser.parse_args()
    png_to_icon_ttf(args.png, args.output, int(args.codepoint, 16), invert=args.invert)


if __name__ == "__main__":
    main()
