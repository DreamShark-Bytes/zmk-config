/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --font /home/vince/Documents/zmk-config/tools/../resources/fonts/BadComic_Font_0_98/TrueType (.ttf)/BadComic-Regular.ttf --range 0x20-0x7F --size 14 --bpp 1 --format lvgl --lv-font-name font_badcomic_14 -o /home/vince/Documents/zmk-config/tools/../resources/fonts/generated/font_badcomic_14.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONT_BADCOMIC_14
#define FONT_BADCOMIC_14 1
#endif

#if FONT_BADCOMIC_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfe, 0x40,

    /* U+0022 "\"" */
    0xb6, 0x80,

    /* U+0023 "#" */
    0x8, 0xb2, 0x9f, 0x29, 0xaf, 0xd6, 0x51, 0x40,

    /* U+0024 "$" */
    0x10, 0x20, 0xf2, 0x85, 0xe, 0x1f, 0xf, 0x12,
    0xad, 0xf1, 0xc1, 0x0,

    /* U+0025 "%" */
    0x73, 0x34, 0x8d, 0x61, 0xd0, 0xc, 0x6, 0xc1,
    0x78, 0xd2, 0x66, 0x90, 0xe0,

    /* U+0026 "&" */
    0x70, 0x50, 0x50, 0x50, 0x70, 0x23, 0x72, 0xde,
    0x8c, 0x8e, 0x7b,

    /* U+0027 "'" */
    0xe0,

    /* U+0028 "(" */
    0x2d, 0x69, 0x24, 0x93, 0x36,

    /* U+0029 ")" */
    0xc9, 0x92, 0x49, 0x25, 0xa4,

    /* U+002A "*" */
    0x10, 0x4f, 0xce, 0x79, 0x20,

    /* U+002B "+" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x8,

    /* U+002C "," */
    0xf8,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x11, 0x33, 0x22, 0x64, 0x4c, 0x80,

    /* U+0030 "0" */
    0x38, 0xcb, 0x1e, 0x18, 0x30, 0x70, 0xa1, 0x64,
    0x70,

    /* U+0031 "1" */
    0x13, 0xf9, 0x11, 0x11, 0x11,

    /* U+0032 "2" */
    0x38, 0x89, 0x12, 0x20, 0xc1, 0x6, 0x18, 0x7d,
    0xfc,

    /* U+0033 "3" */
    0x38, 0x64, 0x44, 0x4, 0x1c, 0x6, 0x2, 0x2,
    0xc6, 0x7c,

    /* U+0034 "4" */
    0x4, 0xc, 0x1c, 0x34, 0x24, 0x64, 0xc4, 0xfe,
    0x4, 0x4, 0x4,

    /* U+0035 "5" */
    0x7e, 0x81, 0x2, 0xf, 0xc8, 0xc0, 0x81, 0xc6,
    0xf8,

    /* U+0036 "6" */
    0x78, 0x9a, 0x14, 0xf, 0xd8, 0xa1, 0xa1, 0x46,
    0x78,

    /* U+0037 "7" */
    0xfc, 0x4, 0x18, 0x20, 0xc3, 0x6, 0x18, 0x20,
    0x40,

    /* U+0038 "8" */
    0x79, 0x9b, 0x16, 0x67, 0x8f, 0xb3, 0x62, 0x84,
    0xf0,

    /* U+0039 "9" */
    0x3d, 0x8a, 0x1c, 0x1c, 0x6f, 0xc0, 0xc3, 0xc4,
    0xf0,

    /* U+003A ":" */
    0x81,

    /* U+003B ";" */
    0x80, 0xb, 0x80,

    /* U+003C "<" */
    0xc, 0xe6, 0x30, 0x70, 0xf0, 0xc0,

    /* U+003D "=" */
    0xfc, 0x0, 0x3f,

    /* U+003E ">" */
    0xc1, 0xc1, 0x83, 0x3b, 0xcc, 0x0,

    /* U+003F "?" */
    0x38, 0x89, 0x9a, 0x30, 0xe3, 0x84, 0x8, 0x0,
    0x20,

    /* U+0040 "@" */
    0x1f, 0x8, 0x24, 0x6, 0x79, 0x96, 0xe5, 0xa9,
    0xfa, 0x0, 0x40, 0x18, 0x1, 0xc0,

    /* U+0041 "A" */
    0x18, 0x18, 0x38, 0x2c, 0x64, 0x7c, 0x46, 0xc2,
    0x83, 0x81,

    /* U+0042 "B" */
    0xf9, 0xa, 0x14, 0x6f, 0xdf, 0xa1, 0x42, 0x85,
    0xf0,

    /* U+0043 "C" */
    0x3c, 0x46, 0x82, 0x80, 0x80, 0x80, 0x81, 0x83,
    0x43, 0x3e,

    /* U+0044 "D" */
    0x78, 0x86, 0x82, 0x83, 0x83, 0x83, 0x83, 0x86,
    0x9c, 0xf0,

    /* U+0045 "E" */
    0xfe, 0x8, 0x20, 0xf2, 0x8, 0x20, 0x83, 0xf0,

    /* U+0046 "F" */
    0xff, 0x2, 0x4, 0xf, 0x10, 0x20, 0x40, 0x81,
    0x0,

    /* U+0047 "G" */
    0x1c, 0x33, 0x10, 0x18, 0x8, 0x4, 0x7a, 0x7,
    0x83, 0x41, 0x9f, 0x0,

    /* U+0048 "H" */
    0x83, 0x6, 0xc, 0x1f, 0xf0, 0x60, 0xc1, 0x83,
    0x4,

    /* U+0049 "I" */
    0xff, 0xc0,

    /* U+004A "J" */
    0x4, 0x10, 0x41, 0x4, 0x10, 0x61, 0x89, 0xc0,

    /* U+004B "K" */
    0x85, 0x1a, 0xc7, 0xe, 0x16, 0x26, 0x46, 0x85,
    0xc,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x81, 0xf0,

    /* U+004D "M" */
    0x43, 0x71, 0xbd, 0xf2, 0x99, 0xcc, 0x46, 0x3,
    0x1, 0x80, 0xc0, 0x40,

    /* U+004E "N" */
    0xc1, 0xc1, 0xe1, 0xa1, 0xb1, 0x99, 0x99, 0x8f,
    0x87, 0x82,

    /* U+004F "O" */
    0x3c, 0x21, 0xb0, 0x50, 0x18, 0xc, 0x6, 0x2,
    0x82, 0x63, 0x1f, 0x0,

    /* U+0050 "P" */
    0xf9, 0x1a, 0x1c, 0x38, 0x7f, 0xa0, 0x40, 0x81,
    0x0,

    /* U+0051 "Q" */
    0x3c, 0x10, 0xc4, 0x12, 0x6, 0x80, 0xa0, 0x28,
    0xe, 0xa, 0x43, 0x88, 0xe1, 0xf8, 0x2,

    /* U+0052 "R" */
    0xf9, 0x1a, 0x14, 0x6f, 0x97, 0x22, 0x46, 0x85,
    0x8,

    /* U+0053 "S" */
    0x3c, 0xc5, 0xb, 0x17, 0x3, 0x81, 0xe1, 0x66,
    0x78,

    /* U+0054 "T" */
    0xff, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
    0x8, 0x8,

    /* U+0055 "U" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xe2, 0x44,
    0x70,

    /* U+0056 "V" */
    0x41, 0x41, 0x41, 0x63, 0x62, 0x22, 0x36, 0x14,
    0x1c, 0x8,

    /* U+0057 "W" */
    0x80, 0x18, 0x41, 0xc6, 0x3c, 0x63, 0x4f, 0x34,
    0xd2, 0x49, 0xa7, 0x8e, 0x30, 0xc3, 0x4,

    /* U+0058 "X" */
    0xc1, 0xb1, 0x8c, 0x83, 0xc0, 0xc0, 0x60, 0x78,
    0x26, 0x31, 0x30, 0xc0,

    /* U+0059 "Y" */
    0x83, 0x8d, 0x13, 0x63, 0x82, 0x4, 0x8, 0x10,
    0x20,

    /* U+005A "Z" */
    0xfe, 0x6, 0x6, 0xc, 0x18, 0x18, 0x30, 0x60,
    0xc0, 0xff,

    /* U+005B "[" */
    0xf2, 0x49, 0x24, 0x92, 0x4e,

    /* U+005C "\\" */
    0x8c, 0x44, 0x66, 0x22, 0x31, 0x10,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x24, 0x9e,

    /* U+005E "^" */
    0x0, 0x60, 0xe3, 0x4c, 0xd8, 0xa1, 0x0,

    /* U+005F "_" */
    0xfc,

    /* U+0060 "`" */
    0x18, 0x80,

    /* U+0061 "a" */
    0x3e, 0x4e, 0x86, 0x86, 0x86, 0x86, 0x4e, 0x7b,

    /* U+0062 "b" */
    0x40, 0x81, 0x2, 0x7, 0x91, 0xa1, 0xc3, 0x87,
    0x1b, 0xe0,

    /* U+0063 "c" */
    0x3c, 0xcf, 0x6, 0x8, 0x18, 0x99, 0x1e,

    /* U+0064 "d" */
    0x2, 0x2, 0x2, 0x2, 0x1e, 0x62, 0x41, 0xc1,
    0x41, 0x61, 0x1f,

    /* U+0065 "e" */
    0x3c, 0x8e, 0xf, 0xfc, 0x18, 0x18, 0x9f,

    /* U+0066 "f" */
    0x36, 0x4f, 0x44, 0x44, 0x44, 0x40,

    /* U+0067 "g" */
    0x3e, 0xcd, 0xe, 0x18, 0x30, 0xf3, 0xbd, 0x3,
    0x89, 0xe0,

    /* U+0068 "h" */
    0x82, 0x8, 0x3c, 0xca, 0x18, 0x61, 0x86, 0x18,
    0x40,

    /* U+0069 "i" */
    0xbf, 0xc0,

    /* U+006A "j" */
    0x41, 0x24, 0xc9, 0x24, 0xbc,

    /* U+006B "k" */
    0x82, 0x8, 0x23, 0x9a, 0xce, 0x38, 0xb2, 0x68,
    0x80,

    /* U+006C "l" */
    0xaa, 0xaa, 0xac,

    /* U+006D "m" */
    0xff, 0xb3, 0x28, 0xc6, 0x11, 0x84, 0x61, 0x18,
    0x46, 0x11,

    /* U+006E "n" */
    0xf2, 0x28, 0x61, 0x86, 0x18, 0x61,

    /* U+006F "o" */
    0x78, 0xcc, 0x86, 0x82, 0x82, 0x82, 0x46, 0x38,

    /* U+0070 "p" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0xe3, 0x3c, 0x40,
    0x81, 0x0,

    /* U+0071 "q" */
    0x1f, 0x71, 0x61, 0xe1, 0x61, 0x62, 0x32, 0x1e,
    0x2, 0x2, 0x2,

    /* U+0072 "r" */
    0xbe, 0x49, 0x24,

    /* U+0073 "s" */
    0x73, 0x2c, 0xbc, 0x3a, 0x3c, 0x9c,

    /* U+0074 "t" */
    0x44, 0xf4, 0x44, 0x44, 0x43,

    /* U+0075 "u" */
    0x83, 0x6, 0xc, 0x18, 0x70, 0xd3, 0xbd,

    /* U+0076 "v" */
    0xc3, 0x8d, 0x1b, 0x22, 0xc5, 0xe, 0x8,

    /* U+0077 "w" */
    0x84, 0x38, 0xcd, 0x39, 0xb7, 0x26, 0xa4, 0x77,
    0x8e, 0x61, 0x8c,

    /* U+0078 "x" */
    0x46, 0xc8, 0xf0, 0xc1, 0x87, 0x99, 0x63,

    /* U+0079 "y" */
    0x87, 0x1c, 0x51, 0x64, 0xb3, 0x86, 0x18, 0xce,
    0x0,

    /* U+007A "z" */
    0xfc, 0x31, 0x8c, 0x31, 0x8c, 0x3f,

    /* U+007B "{" */
    0x74, 0x44, 0x4c, 0xc4, 0x44, 0x46, 0x0,

    /* U+007C "|" */
    0xff, 0xfe,

    /* U+007D "}" */
    0xc9, 0x24, 0xd2, 0x49, 0x60,

    /* U+007E "~" */
    0xe6, 0xf1, 0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 46, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 56, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 80, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 5, .adv_w = 135, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 13, .adv_w = 135, .box_w = 7, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 25, .adv_w = 168, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 38, .adv_w = 149, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 49, .adv_w = 42, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 50, .adv_w = 75, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 55, .adv_w = 75, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 60, .adv_w = 129, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 65, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 52, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 73, .adv_w = 135, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 74, .adv_w = 52, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 70, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 81, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 90, .adv_w = 135, .box_w = 4, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 95, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 135, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 114, .adv_w = 135, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 134, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 143, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 170, .adv_w = 52, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 52, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 174, .adv_w = 135, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 180, .adv_w = 135, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 183, .adv_w = 135, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 189, .adv_w = 117, .box_w = 7, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 198, .adv_w = 195, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 212, .adv_w = 147, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 222, .adv_w = 144, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 231, .adv_w = 159, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 241, .adv_w = 161, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 251, .adv_w = 134, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 259, .adv_w = 130, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 268, .adv_w = 162, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 166, .box_w = 7, .box_h = 10, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 62, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 291, .adv_w = 127, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 144, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 132, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 197, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 328, .adv_w = 165, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 338, .adv_w = 168, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 350, .adv_w = 137, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 359, .adv_w = 168, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 374, .adv_w = 142, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 133, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 392, .adv_w = 138, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 402, .adv_w = 155, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 411, .adv_w = 146, .box_w = 8, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 421, .adv_w = 217, .box_w = 12, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 145, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 135, .box_w = 7, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 457, .adv_w = 136, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 467, .adv_w = 75, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 472, .adv_w = 70, .box_w = 4, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 478, .adv_w = 73, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 483, .adv_w = 135, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 490, .adv_w = 112, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 491, .adv_w = 67, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 11},
    {.bitmap_index = 493, .adv_w = 122, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 501, .adv_w = 140, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 511, .adv_w = 122, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 518, .adv_w = 141, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 529, .adv_w = 126, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 536, .adv_w = 70, .box_w = 4, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 542, .adv_w = 141, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 552, .adv_w = 136, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 561, .adv_w = 56, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 563, .adv_w = 55, .box_w = 3, .box_h = 13, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 568, .adv_w = 123, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 577, .adv_w = 59, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 580, .adv_w = 198, .box_w = 10, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 590, .adv_w = 136, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 133, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 604, .adv_w = 140, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 614, .adv_w = 141, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 625, .adv_w = 79, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 107, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 634, .adv_w = 75, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 639, .adv_w = 135, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 646, .adv_w = 121, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 653, .adv_w = 181, .box_w = 11, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 664, .adv_w = 114, .box_w = 7, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 671, .adv_w = 120, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 680, .adv_w = 107, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 686, .adv_w = 75, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 693, .adv_w = 60, .box_w = 1, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 695, .adv_w = 75, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 700, .adv_w = 135, .box_w = 6, .box_h = 3, .ofs_x = 1, .ofs_y = 3}
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
const lv_font_t font_badcomic_14 = {
#else
lv_font_t font_badcomic_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 17,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if FONT_BADCOMIC_14*/

