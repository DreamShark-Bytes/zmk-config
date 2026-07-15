/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --font /home/vince/Documents/zmk-config/tools/../resources/fonts/BadComic_Font_0_98/TrueType (.ttf)/BadComic-Regular.ttf --range 0x20-0x7F --size 12 --bpp 1 --format lvgl --lv-font-name font_badcomic_12 -o /home/vince/Documents/zmk-config/tools/../resources/fonts/generated/font_badcomic_12.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONT_BADCOMIC_12
#define FONT_BADCOMIC_12 1
#endif

#if FONT_BADCOMIC_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfc, 0x80,

    /* U+0022 "\"" */
    0xb4,

    /* U+0023 "#" */
    0x8, 0xa2, 0x9f, 0x2b, 0xf5, 0x94, 0x50,

    /* U+0024 "$" */
    0x20, 0x87, 0xa8, 0xa3, 0xc3, 0x8a, 0xbb, 0xc2,
    0x0,

    /* U+0025 "%" */
    0xe3, 0xa2, 0xa6, 0x64, 0x8, 0x1e, 0x1d, 0x35,
    0x66,

    /* U+0026 "&" */
    0x60, 0xa1, 0x43, 0x86, 0x6e, 0xa7, 0x46, 0x76,

    /* U+0027 "'" */
    0xe0,

    /* U+0028 "(" */
    0x4b, 0x49, 0x24, 0x89, 0x80,

    /* U+0029 ")" */
    0xc8, 0x92, 0x49, 0x2d, 0x0,

    /* U+002A "*" */
    0x25, 0x3e, 0xe5, 0x0,

    /* U+002B "+" */
    0x21, 0x9, 0xf2, 0x10, 0x80,

    /* U+002C "," */
    0xd8,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x13, 0x22, 0x64, 0x44, 0x88,

    /* U+0030 "0" */
    0x31, 0x28, 0x61, 0x86, 0x1c, 0x53, 0x38,

    /* U+0031 "1" */
    0x13, 0x51, 0x11, 0x11, 0x10,

    /* U+0032 "2" */
    0x79, 0x24, 0xc2, 0x18, 0x42, 0x1c, 0xfc,

    /* U+0033 "3" */
    0x72, 0x20, 0x8c, 0x8, 0x20, 0xa2, 0xf0,

    /* U+0034 "4" */
    0x8, 0x63, 0x9a, 0x4b, 0x2f, 0xc2, 0x8,

    /* U+0035 "5" */
    0x7d, 0x4, 0x3e, 0xc4, 0x18, 0x73, 0x78,

    /* U+0036 "6" */
    0x73, 0x68, 0xe0, 0xfb, 0x38, 0x51, 0x38,

    /* U+0037 "7" */
    0xf8, 0x42, 0x23, 0x11, 0x88, 0x40,

    /* U+0038 "8" */
    0x72, 0x28, 0xbe, 0x31, 0x6c, 0xe3, 0x78,

    /* U+0039 "9" */
    0x7b, 0x38, 0x71, 0x7c, 0x18, 0x72, 0x78,

    /* U+003A ":" */
    0x82,

    /* U+003B ";" */
    0x40, 0x37,

    /* U+003C "<" */
    0x19, 0xb9, 0xc3, 0x84,

    /* U+003D "=" */
    0xf8, 0x1, 0xf0,

    /* U+003E ">" */
    0xc3, 0x6, 0x36, 0x40,

    /* U+003F "?" */
    0x72, 0x2c, 0x82, 0x18, 0xc2, 0x0, 0x30,

    /* U+0040 "@" */
    0x1e, 0x30, 0x90, 0x53, 0xaa, 0x54, 0xf2, 0x0,
    0x80, 0x60, 0x1e, 0x0,

    /* U+0041 "A" */
    0x10, 0x60, 0xc2, 0x47, 0x88, 0xa1, 0x42, 0x82,

    /* U+0042 "B" */
    0xfa, 0x28, 0xbe, 0xfa, 0x28, 0xe2, 0xf0,

    /* U+0043 "C" */
    0x78, 0x8a, 0x14, 0x8, 0x10, 0x21, 0x42, 0x7c,

    /* U+0044 "D" */
    0x79, 0x1a, 0x1c, 0x18, 0x30, 0xe1, 0x4c, 0xf0,

    /* U+0045 "E" */
    0xfc, 0x21, 0xf, 0x42, 0x10, 0xf8,

    /* U+0046 "F" */
    0xfe, 0x8, 0x3c, 0x82, 0x8, 0x20, 0x80,

    /* U+0047 "G" */
    0x38, 0x64, 0x40, 0x80, 0x80, 0x9e, 0x81, 0x43,
    0x3e,

    /* U+0048 "H" */
    0x86, 0x18, 0x7f, 0x86, 0x18, 0x61, 0x84,

    /* U+0049 "I" */
    0xff, 0x80,

    /* U+004A "J" */
    0x8, 0x42, 0x10, 0x86, 0x32, 0x70,

    /* U+004B "K" */
    0x8a, 0x6b, 0x30, 0xe2, 0xc9, 0xa3, 0x84,

    /* U+004C "L" */
    0x84, 0x21, 0x8, 0x42, 0x10, 0x78,

    /* U+004D "M" */
    0x42, 0xe6, 0xbd, 0x99, 0x99, 0x81, 0x81, 0x81,
    0x81,

    /* U+004E "N" */
    0xc3, 0x87, 0x8d, 0x1b, 0x32, 0x66, 0xc7, 0x84,

    /* U+004F "O" */
    0x38, 0x46, 0x82, 0x82, 0x83, 0x82, 0x82, 0x46,
    0x38,

    /* U+0050 "P" */
    0xf2, 0x28, 0xe3, 0x8f, 0xe8, 0x20, 0x80,

    /* U+0051 "Q" */
    0x3c, 0x46, 0x82, 0x81, 0x81, 0x81, 0x45, 0x66,
    0x3f, 0x0,

    /* U+0052 "R" */
    0xf2, 0x28, 0xa2, 0xf2, 0xc9, 0xa2, 0x8c,

    /* U+0053 "S" */
    0x39, 0x1c, 0x58, 0x30, 0x7c, 0x59, 0x38,

    /* U+0054 "T" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,

    /* U+0055 "U" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x52, 0x30,

    /* U+0056 "V" */
    0x83, 0x85, 0xa, 0x36, 0x44, 0x8a, 0x1c, 0x18,

    /* U+0057 "W" */
    0xc0, 0x28, 0x85, 0x18, 0xa3, 0x36, 0xe4, 0x52,
    0x8a, 0x51, 0x86, 0x10, 0xc0,

    /* U+0058 "X" */
    0xc2, 0x66, 0x34, 0x1c, 0x8, 0x1c, 0x34, 0x62,
    0x43,

    /* U+0059 "Y" */
    0x82, 0x8d, 0xb1, 0x41, 0x2, 0x4, 0x8, 0x10,

    /* U+005A "Z" */
    0xfc, 0x30, 0x86, 0x30, 0x84, 0x30, 0xfc,

    /* U+005B "[" */
    0xea, 0xaa, 0xac,

    /* U+005C "\\" */
    0x88, 0x44, 0x46, 0x22, 0x31,

    /* U+005D "]" */
    0xd5, 0x55, 0x5c,

    /* U+005E "^" */
    0x1, 0x95, 0xb8, 0xc4,

    /* U+005F "_" */
    0xfc,

    /* U+0060 "`" */
    0x19, 0x80,

    /* U+0061 "a" */
    0x7c, 0x9a, 0x14, 0x28, 0xd9, 0x9d, 0x80,

    /* U+0062 "b" */
    0x82, 0x8, 0x3c, 0x8a, 0x18, 0x63, 0xf0,

    /* U+0063 "c" */
    0x39, 0x18, 0x20, 0x81, 0x23, 0x80,

    /* U+0064 "d" */
    0x4, 0x10, 0x4f, 0x65, 0x1c, 0x51, 0x3c,

    /* U+0065 "e" */
    0x7b, 0x38, 0x7e, 0x81, 0x3, 0xc0,

    /* U+0066 "f" */
    0x34, 0x4f, 0x44, 0x44, 0x44,

    /* U+0067 "g" */
    0x3d, 0x3c, 0x61, 0x87, 0x37, 0x41, 0xcd, 0xe0,

    /* U+0068 "h" */
    0x84, 0x21, 0xe9, 0x46, 0x31, 0x8c, 0x40,

    /* U+0069 "i" */
    0xbf, 0x80,

    /* U+006A "j" */
    0x45, 0x55, 0x56,

    /* U+006B "k" */
    0x84, 0x23, 0x2a, 0x62, 0x92, 0x88,

    /* U+006C "l" */
    0xaa, 0xaa, 0xc0,

    /* U+006D "m" */
    0xff, 0x6c, 0xe2, 0x31, 0x18, 0x8c, 0x46, 0x22,

    /* U+006E "n" */
    0xf2, 0x28, 0x61, 0x86, 0x18, 0x40,

    /* U+006F "o" */
    0x71, 0x1a, 0x14, 0x28, 0x49, 0x8e, 0x0,

    /* U+0070 "p" */
    0xf2, 0x38, 0x61, 0x8e, 0x6f, 0x20, 0x82, 0x0,

    /* U+0071 "q" */
    0x3d, 0x18, 0x61, 0x45, 0x13, 0xc1, 0x4, 0x10,

    /* U+0072 "r" */
    0xfa, 0x49, 0x20,

    /* U+0073 "s" */
    0x74, 0xb0, 0xe1, 0x4b, 0xc0,

    /* U+0074 "t" */
    0x44, 0xf4, 0x44, 0x44, 0x30,

    /* U+0075 "u" */
    0x86, 0x18, 0x61, 0x8e, 0x37, 0x40,

    /* U+0076 "v" */
    0x87, 0x14, 0xd2, 0x28, 0xc1, 0x0,

    /* U+0077 "w" */
    0x88, 0xcc, 0x77, 0x6a, 0xa5, 0x53, 0xb8, 0x88,

    /* U+0078 "x" */
    0xcd, 0xa3, 0x84, 0x39, 0xac, 0xc0,

    /* U+0079 "y" */
    0x45, 0x14, 0x59, 0x24, 0xf1, 0x82, 0x13, 0x80,

    /* U+007A "z" */
    0xf8, 0xcc, 0xc4, 0x63, 0xe0,

    /* U+007B "{" */
    0x69, 0x24, 0x92, 0x48, 0x80,

    /* U+007C "|" */
    0xff, 0xf8,

    /* U+007D "}" */
    0xc9, 0x24, 0xd2, 0x4b, 0x0,

    /* U+007E "~" */
    0x65, 0xd1, 0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 40, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 48, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3, .adv_w = 69, .box_w = 3, .box_h = 2, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 4, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 11, .adv_w = 115, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 20, .adv_w = 144, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 29, .adv_w = 127, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 37, .adv_w = 36, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 38, .adv_w = 65, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 43, .adv_w = 65, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 48, .adv_w = 111, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 52, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 57, .adv_w = 45, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 58, .adv_w = 115, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 59, .adv_w = 45, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 60, .adv_w = 60, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 65, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 115, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 91, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 98, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 132, .adv_w = 45, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 45, .box_w = 2, .box_h = 8, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 135, .adv_w = 115, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 115, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 142, .adv_w = 115, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 100, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 167, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 165, .adv_w = 126, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 173, .adv_w = 123, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 180, .adv_w = 136, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 188, .adv_w = 138, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 196, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 111, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 209, .adv_w = 139, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 142, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 53, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 227, .adv_w = 109, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 123, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 113, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 246, .adv_w = 169, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 255, .adv_w = 142, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 144, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 272, .adv_w = 117, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 279, .adv_w = 144, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 289, .adv_w = 122, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 114, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 118, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 311, .adv_w = 133, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 318, .adv_w = 125, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 186, .box_w = 11, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 339, .adv_w = 124, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 348, .adv_w = 116, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 356, .adv_w = 116, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 363, .adv_w = 64, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 366, .adv_w = 60, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 371, .adv_w = 62, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 374, .adv_w = 115, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 378, .adv_w = 96, .box_w = 6, .box_h = 1, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 379, .adv_w = 58, .box_w = 3, .box_h = 3, .ofs_x = 0, .ofs_y = 10},
    {.bitmap_index = 381, .adv_w = 105, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 388, .adv_w = 120, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 104, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 401, .adv_w = 121, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 108, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 414, .adv_w = 60, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 419, .adv_w = 121, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 427, .adv_w = 116, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 48, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 47, .box_w = 2, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 439, .adv_w = 106, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 445, .adv_w = 50, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 170, .box_w = 9, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 456, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 462, .adv_w = 114, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 469, .adv_w = 120, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 477, .adv_w = 121, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 485, .adv_w = 68, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 488, .adv_w = 91, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 493, .adv_w = 64, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 116, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 504, .adv_w = 103, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 155, .box_w = 9, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 518, .adv_w = 98, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 524, .adv_w = 103, .box_w = 6, .box_h = 10, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 532, .adv_w = 92, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 537, .adv_w = 64, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 542, .adv_w = 51, .box_w = 1, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 544, .adv_w = 64, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 549, .adv_w = 115, .box_w = 6, .box_h = 3, .ofs_x = 0, .ofs_y = 2}
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
const lv_font_t font_badcomic_12 = {
#else
lv_font_t font_badcomic_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
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



#endif /*#if FONT_BADCOMIC_12*/

