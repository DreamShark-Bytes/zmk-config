/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --font /home/vince/Documents/zmk-config/tools/../resources/fonts/BadComic_Font_0_98/TrueType (.ttf)/BadComic-Regular.ttf --range 0x20-0x7F --size 16 --bpp 1 --format lvgl --lv-font-name font_badcomic_16 -o /home/vince/Documents/zmk-config/tools/../resources/fonts/generated/font_badcomic_16.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONT_BADCOMIC_16
#define FONT_BADCOMIC_16 1
#endif

#if FONT_BADCOMIC_16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xfc, 0x3,

    /* U+0022 "\"" */
    0xff, 0xf0,

    /* U+0023 "#" */
    0x4, 0x28, 0xd3, 0xa7, 0xe5, 0x8b, 0x7f, 0x68,
    0xd1, 0x22, 0x40,

    /* U+0024 "$" */
    0x18, 0x18, 0x3e, 0x58, 0xd8, 0xd8, 0x7c, 0x1e,
    0x1a, 0x1a, 0xda, 0xfe, 0x38, 0x18,

    /* U+0025 "%" */
    0x70, 0x9f, 0x33, 0x6c, 0x6d, 0x87, 0x60, 0xc,
    0x3, 0x60, 0xde, 0x1b, 0x66, 0x6d, 0x8d, 0xa0,
    0xe0,

    /* U+0026 "&" */
    0x78, 0x24, 0x12, 0xf, 0x7, 0x81, 0x85, 0xe6,
    0xf3, 0xcf, 0x63, 0x33, 0xcf, 0xb0,

    /* U+0027 "'" */
    0xfc,

    /* U+0028 "(" */
    0x26, 0x64, 0xcc, 0xcc, 0xcc, 0xc6, 0x31,

    /* U+0029 ")" */
    0xcc, 0x62, 0x33, 0x33, 0x33, 0x36, 0x64,

    /* U+002A "*" */
    0x18, 0x33, 0xfb, 0xf3, 0xcd, 0xc0, 0x0,

    /* U+002B "+" */
    0x18, 0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18,
    0x18,

    /* U+002C "," */
    0xcb, 0x0,

    /* U+002D "-" */
    0xfe,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0x8, 0xc4, 0x63, 0x18, 0x8c, 0x62, 0x31, 0x88,
    0x0,

    /* U+0030 "0" */
    0x1c, 0x31, 0x90, 0xd8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0x41, 0xb0, 0xc8, 0xc3, 0xc0,

    /* U+0031 "1" */
    0x19, 0xdf, 0xb9, 0x8c, 0x63, 0x18, 0xc6, 0x30,

    /* U+0032 "2" */
    0x3c, 0x66, 0xc2, 0x42, 0x6, 0x6, 0xc, 0x1c,
    0x18, 0x30, 0xfe, 0xcf,

    /* U+0033 "3" */
    0x79, 0x8b, 0x16, 0x20, 0xc7, 0x81, 0x81, 0x2,
    0x7, 0x13, 0xc0,

    /* U+0034 "4" */
    0x3, 0x3, 0x83, 0xc3, 0x61, 0xb1, 0x99, 0x8c,
    0xc6, 0xff, 0x81, 0x80, 0xc0, 0x60,

    /* U+0035 "5" */
    0x7f, 0x40, 0xc0, 0xc0, 0xc0, 0xfe, 0xc3, 0x3,
    0x3, 0x83, 0xc7, 0x7c,

    /* U+0036 "6" */
    0x1c, 0x7e, 0x63, 0xc1, 0xc0, 0xfe, 0xe6, 0xc3,
    0xc3, 0x63, 0x23, 0x1e,

    /* U+0037 "7" */
    0xfe, 0x3, 0x3, 0x7, 0x6, 0xc, 0xc, 0x18,
    0x18, 0x30, 0x30, 0x20,

    /* U+0038 "8" */
    0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x7c, 0x3c, 0x66,
    0xc2, 0xc2, 0xc2, 0x7c,

    /* U+0039 "9" */
    0x3c, 0x62, 0xc3, 0xc3, 0xc3, 0x43, 0x3f, 0x3,
    0x3, 0xc3, 0xc6, 0x3c,

    /* U+003A ":" */
    0xc0, 0x0, 0xc0,

    /* U+003B ";" */
    0xc0, 0xf, 0xe0,

    /* U+003C "<" */
    0x6, 0x3d, 0xc7, 0xe, 0xf, 0x7, 0x83,

    /* U+003D "=" */
    0x7e, 0x0, 0x0, 0xf, 0xc0,

    /* U+003E ">" */
    0xc1, 0xe0, 0x70, 0x70, 0xe7, 0xbc, 0x60,

    /* U+003F "?" */
    0x38, 0x66, 0xc2, 0x62, 0x2, 0x6, 0xc, 0x18,
    0x18, 0x18, 0x0, 0x18,

    /* U+0040 "@" */
    0xf, 0x83, 0x86, 0x60, 0x66, 0x2, 0xcf, 0x2d,
    0x92, 0xd9, 0x6c, 0xfc, 0xc0, 0x6, 0x0, 0x60,
    0x3, 0x80, 0xf, 0x0,

    /* U+0041 "A" */
    0x8, 0xe, 0x7, 0x6, 0x83, 0x61, 0x31, 0xf8,
    0x86, 0x41, 0x60, 0xf0, 0x70, 0x10,

    /* U+0042 "B" */
    0x7e, 0xc7, 0xc3, 0xc3, 0xcf, 0xfe, 0xfe, 0xc3,
    0xc3, 0xc3, 0xc2, 0xfc,

    /* U+0043 "C" */
    0x3e, 0x31, 0xb8, 0x58, 0x2c, 0x6, 0x3, 0x1,
    0x80, 0xc1, 0xe0, 0xd8, 0x67, 0xe0,

    /* U+0044 "D" */
    0x7c, 0x23, 0x30, 0xd8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0x61, 0xb3, 0x1e, 0x0,

    /* U+0045 "E" */
    0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xff,

    /* U+0046 "F" */
    0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0047 "G" */
    0xf, 0x3, 0x20, 0xc6, 0x30, 0x6, 0x1, 0x80,
    0x30, 0x6, 0x3e, 0xc0, 0x4c, 0x9, 0xc3, 0xf,
    0xc0,

    /* U+0048 "H" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1f, 0xff, 0x7,
    0x83, 0xc1, 0xe0, 0xf0, 0x78, 0x30,

    /* U+0049 "I" */
    0xff, 0xff, 0xff,

    /* U+004A "J" */
    0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x7,
    0x8b, 0x33, 0xc0,

    /* U+004B "K" */
    0xc3, 0x63, 0xb3, 0x1b, 0xf, 0x7, 0x83, 0x61,
    0x98, 0xc6, 0x61, 0xb0, 0x58, 0x30,

    /* U+004C "L" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0x40, 0x3e,

    /* U+004D "M" */
    0x60, 0x87, 0x1c, 0x73, 0xc5, 0xb6, 0xce, 0x6c,
    0xe6, 0xc4, 0x6c, 0x6, 0xc0, 0x6c, 0x6, 0xc0,
    0x6c, 0x2,

    /* U+004E "N" */
    0x41, 0xb0, 0xd8, 0x6c, 0x35, 0x1e, 0x8f, 0x67,
    0x93, 0xcd, 0xe3, 0xb0, 0xd8, 0x60,

    /* U+004F "O" */
    0x1e, 0x18, 0xc6, 0x1b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xd8, 0x66, 0x30, 0x78,

    /* U+0050 "P" */
    0xf8, 0xc6, 0xc3, 0xc1, 0xc1, 0xc1, 0xc2, 0xfc,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0051 "Q" */
    0x1e, 0xc, 0x31, 0x83, 0x60, 0x6c, 0x7, 0x80,
    0xf0, 0x1e, 0x3, 0x63, 0x6c, 0x38, 0xc7, 0xf,
    0xe0, 0x6,

    /* U+0052 "R" */
    0xfc, 0xc6, 0xc2, 0xc2, 0xc2, 0xc6, 0xfc, 0xdc,
    0xc6, 0xc6, 0xc3, 0xc3,

    /* U+0053 "S" */
    0x1e, 0x67, 0x43, 0xc3, 0x60, 0x70, 0x3c, 0xe,
    0x83, 0xc3, 0x63, 0x1c,

    /* U+0054 "T" */
    0xff, 0x86, 0x3, 0x1, 0x80, 0xc0, 0x60, 0x30,
    0x18, 0xc, 0x6, 0x3, 0x1, 0x80,

    /* U+0055 "U" */
    0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0x41, 0x31, 0x88, 0xc3, 0x80,

    /* U+0056 "V" */
    0x81, 0xc0, 0xf0, 0x78, 0x3c, 0x32, 0x19, 0x8c,
    0xcc, 0x26, 0x1e, 0x7, 0x1, 0x0,

    /* U+0057 "W" */
    0x40, 0x6, 0x80, 0xd, 0x4, 0x1b, 0x1c, 0x36,
    0x38, 0x6c, 0x79, 0x88, 0xb3, 0x1b, 0x36, 0x34,
    0x6c, 0x38, 0x70, 0x60, 0x60, 0x40, 0xc0,

    /* U+0058 "X" */
    0xc0, 0xd8, 0x33, 0x18, 0x6c, 0xf, 0x3, 0x80,
    0x60, 0x3c, 0x1b, 0x6, 0x67, 0xd, 0x81,

    /* U+0059 "Y" */
    0xc0, 0xb0, 0x66, 0x18, 0x8c, 0x36, 0x7, 0x80,
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30,

    /* U+005A "Z" */
    0xff, 0x81, 0x80, 0xc0, 0xe0, 0x60, 0x60, 0x70,
    0x70, 0x30, 0x30, 0x38, 0x1f, 0xf0,

    /* U+005B "[" */
    0xfb, 0x6d, 0xb6, 0xdb, 0x6d, 0xc0,

    /* U+005C "\\" */
    0x86, 0x30, 0x86, 0x30, 0x86, 0x30, 0x86, 0x30,
    0x80,

    /* U+005D "]" */
    0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3f,

    /* U+005E "^" */
    0x0, 0x18, 0x3c, 0x2c, 0x66, 0x46, 0xc2, 0xc3,

    /* U+005F "_" */
    0xfe,

    /* U+0060 "`" */
    0xc6, 0x20,

    /* U+0061 "a" */
    0x2, 0x1f, 0x19, 0x98, 0x6c, 0x36, 0x1b, 0x1c,
    0xce, 0x39, 0x0,

    /* U+0062 "b" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xf8, 0xc6, 0xc2, 0xc1,
    0xc1, 0xc1, 0xc2, 0xfc,

    /* U+0063 "c" */
    0x1e, 0x63, 0x41, 0xc0, 0xc0, 0xc0, 0xc2, 0x62,
    0x3c,

    /* U+0064 "d" */
    0x3, 0x3, 0x3, 0x3, 0x1f, 0x63, 0xc3, 0xc3,
    0x83, 0xc3, 0x63, 0x3f,

    /* U+0065 "e" */
    0x3e, 0x63, 0xc1, 0xc1, 0xfe, 0xc0, 0x40, 0x61,
    0x1f,

    /* U+0066 "f" */
    0x39, 0x99, 0xf6, 0x31, 0x8c, 0x63, 0x18, 0xc0,

    /* U+0067 "g" */
    0x1f, 0x99, 0xd8, 0x68, 0x34, 0x1e, 0xd, 0xe,
    0xcf, 0x3d, 0x80, 0xd0, 0x4c, 0x63, 0xc0,

    /* U+0068 "h" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xe6, 0xc2, 0xc3,
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3,

    /* U+0069 "i" */
    0xc3, 0xff, 0xff,

    /* U+006A "j" */
    0x30, 0x3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x2e,

    /* U+006B "k" */
    0xc1, 0x83, 0x6, 0x1c, 0x79, 0xb6, 0x78, 0xf9,
    0x9b, 0x1e, 0x10,

    /* U+006C "l" */
    0xdb, 0x6d, 0xb6, 0xdb, 0x30,

    /* U+006D "m" */
    0xff, 0xce, 0xc6, 0xc4, 0x3c, 0x63, 0xc6, 0x3c,
    0x63, 0xc6, 0x3c, 0x63, 0xc6, 0x20,

    /* U+006E "n" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3,

    /* U+006F "o" */
    0x3c, 0x33, 0x30, 0xd8, 0x2c, 0x16, 0xb, 0x84,
    0xc4, 0x3c, 0x0,

    /* U+0070 "p" */
    0xf8, 0xc6, 0xc3, 0xc1, 0xc1, 0xc3, 0xc2, 0xc6,
    0xf8, 0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0071 "q" */
    0x1f, 0x63, 0x83, 0x83, 0x83, 0x83, 0x43, 0x63,
    0x1f, 0x3, 0x3, 0x3, 0x3,

    /* U+0072 "r" */
    0xdf, 0xb9, 0x8c, 0x63, 0x18, 0x80,

    /* U+0073 "s" */
    0x7b, 0x3c, 0xfc, 0x78, 0x38, 0xf3, 0x78,

    /* U+0074 "t" */
    0x3, 0x19, 0xf6, 0x31, 0x8c, 0x63, 0x18, 0x70,

    /* U+0075 "u" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xc7, 0x6f,
    0x7b,

    /* U+0076 "v" */
    0xc1, 0xc3, 0x63, 0x62, 0x26, 0x36, 0x3c, 0x1c,
    0x18,

    /* U+0077 "w" */
    0x84, 0x3c, 0x63, 0xce, 0x34, 0xf2, 0x4f, 0x66,
    0xb6, 0x79, 0x43, 0x1c, 0x30, 0x80,

    /* U+0078 "x" */
    0xc7, 0xcd, 0xb1, 0xc1, 0x87, 0x1b, 0x73, 0xc6,

    /* U+0079 "y" */
    0xc3, 0xc3, 0x43, 0x63, 0x63, 0x23, 0x36, 0x36,
    0x1e, 0xc, 0xc, 0x18, 0xf0,

    /* U+007A "z" */
    0xfe, 0x1c, 0x30, 0xc3, 0x86, 0x18, 0x60, 0xfe,

    /* U+007B "{" */
    0x76, 0x66, 0x66, 0xc6, 0x66, 0x66, 0x66,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xff, 0xc0,

    /* U+007D "}" */
    0xc6, 0x66, 0x66, 0x36, 0x66, 0x66, 0xec,

    /* U+007E "~" */
    0x1, 0xee, 0x78, 0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 53, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 65, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 91, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 6, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 17, .adv_w = 154, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 31, .adv_w = 192, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 48, .adv_w = 170, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 48, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 63, .adv_w = 86, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 70, .adv_w = 86, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 77, .adv_w = 148, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 84, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 93, .adv_w = 59, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 95, .adv_w = 154, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 96, .adv_w = 59, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 97, .adv_w = 80, .box_w = 5, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 106, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 154, .box_w = 5, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 128, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 201, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 59, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 228, .adv_w = 59, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 231, .adv_w = 154, .box_w = 7, .box_h = 8, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 238, .adv_w = 154, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 243, .adv_w = 154, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 250, .adv_w = 134, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 262, .adv_w = 223, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 282, .adv_w = 168, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 164, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 181, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 184, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 336, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 148, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 360, .adv_w = 185, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 189, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 391, .adv_w = 71, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 394, .adv_w = 145, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 405, .adv_w = 165, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 419, .adv_w = 151, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 431, .adv_w = 225, .box_w = 12, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 449, .adv_w = 189, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 463, .adv_w = 192, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 478, .adv_w = 156, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 490, .adv_w = 191, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 508, .adv_w = 163, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 520, .adv_w = 152, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 532, .adv_w = 158, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 546, .adv_w = 177, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 167, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 574, .adv_w = 248, .box_w = 15, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 166, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 612, .adv_w = 154, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 627, .adv_w = 155, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 641, .adv_w = 85, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 647, .adv_w = 80, .box_w = 5, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 656, .adv_w = 83, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 663, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 671, .adv_w = 128, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 672, .adv_w = 77, .box_w = 4, .box_h = 3, .ofs_x = 0, .ofs_y = 12},
    {.bitmap_index = 674, .adv_w = 140, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 685, .adv_w = 160, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 697, .adv_w = 139, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 706, .adv_w = 161, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 718, .adv_w = 144, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 727, .adv_w = 80, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 735, .adv_w = 161, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 750, .adv_w = 155, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 763, .adv_w = 64, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 766, .adv_w = 63, .box_w = 4, .box_h = 16, .ofs_x = -1, .ofs_y = -4},
    {.bitmap_index = 774, .adv_w = 141, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 785, .adv_w = 67, .box_w = 3, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 790, .adv_w = 226, .box_w = 12, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 804, .adv_w = 155, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 813, .adv_w = 152, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 824, .adv_w = 160, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 837, .adv_w = 161, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 850, .adv_w = 90, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 856, .adv_w = 122, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 863, .adv_w = 86, .box_w = 5, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 871, .adv_w = 155, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 880, .adv_w = 138, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 889, .adv_w = 206, .box_w = 12, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 903, .adv_w = 130, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 911, .adv_w = 137, .box_w = 8, .box_h = 13, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 924, .adv_w = 122, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 932, .adv_w = 86, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 939, .adv_w = 68, .box_w = 2, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 944, .adv_w = 86, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 951, .adv_w = 154, .box_w = 7, .box_h = 4, .ofs_x = 2, .ofs_y = 3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t font_badcomic_16 = {
#else
lv_font_t font_badcomic_16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 19,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if FONT_BADCOMIC_16*/

