#!/usr/bin/env bash
# tools/build_font.sh
#
# Builds all LVGL font .c files for the custom display module.
# Run this script after adding new icons or changing font sizes.
# Output goes to resources/fonts/generated/.
#
# Requires:
#   lv_font_conv  (npm install -g lv_font_conv)
#   python3 + Pillow + fonttools  (pip3 install Pillow fonttools)
#
# Usage:
#   bash tools/build_font.sh

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$SCRIPT_DIR/.."
FONT_SRC="$ROOT/resources/fonts/BadComic_Font_0_98/TrueType (.ttf)/BadComic-Regular.ttf"
OUT="$ROOT/resources/fonts/generated"
ICONS="$ROOT/resources/icons"
TOOLS="$ROOT/tools"
TMP="$(mktemp -d)"

mkdir -p "$OUT"

# ---------------------------------------------------------------------------
# ICON CODE POINT ASSIGNMENTS (U+E000 private-use range)
# Add new status-string icons here and update display_config.h to match.
# ---------------------------------------------------------------------------
# U+E001  icon_currency   — keycount / currency symbol
# (future) U+E002  icon_hungry
# (future) U+E003  icon_angry
# ---------------------------------------------------------------------------

echo "Building icon TTFs..."
python3 "$TOOLS/png_to_icon_font.py" \
    "$ICONS/icon_currency.png" "$TMP/icon_currency.ttf" --codepoint 0xE001

# ---------------------------------------------------------------------------
# FONT BUILDS
# Sizes: 8 (layer colon), 9 (layer name), 11 (status string), 12 (battery/BT/layer L)
# Size 11 includes inline status icons (currency etc.)
# Full printable ASCII (0x20-0x7F) included in every size.
# ---------------------------------------------------------------------------

echo "Building font_badcomic_8..."
lv_font_conv \
    --font "$FONT_SRC" --range "0x20-0x7F" \
    --size 8 --bpp 1 --format lvgl \
    --lv-font-name font_badcomic_8 \
    -o "$OUT/font_badcomic_8.c"

echo "Building font_badcomic_9..."
lv_font_conv \
    --font "$FONT_SRC" --range "0x20-0x7F" \
    --size 9 --bpp 1 --format lvgl \
    --lv-font-name font_badcomic_9 \
    -o "$OUT/font_badcomic_9.c"

echo "Building font_badcomic_11 (with status icons)..."
lv_font_conv \
    --font "$FONT_SRC" --range "0x20-0x7F" \
    --font "$TMP/icon_currency.ttf" --range "0xE001" \
    --size 11 --bpp 1 --format lvgl \
    --lv-font-name font_badcomic_11 \
    -o "$OUT/font_badcomic_11.c"

echo "Building font_badcomic_12..."
lv_font_conv \
    --font "$FONT_SRC" --range "0x20-0x7F" \
    --size 12 --bpp 1 --format lvgl \
    --lv-font-name font_badcomic_12 \
    -o "$OUT/font_badcomic_12.c"

# ---------------------------------------------------------------------------
# FAKE BOLD (1px right-shift + OR bitmaps)
# Applied to sizes 11 and 12. Build-time only — zero runtime cost.
# To disable: comment out these lines and remove _bold variants from
# display_config.h font selections.
# TODO: implement tools/apply_fake_bold.py
# ---------------------------------------------------------------------------
# python3 "$TOOLS/apply_fake_bold.py" "$OUT/font_badcomic_11.c" "$OUT/font_badcomic_11_bold.c"
# python3 "$TOOLS/apply_fake_bold.py" "$OUT/font_badcomic_12.c" "$OUT/font_badcomic_12_bold.c"

rm -rf "$TMP"
echo ""
echo "Done. Generated files in $OUT:"
ls "$OUT/"
