#pragma once
#include <lvgl.h>
// Generated from resources/icons/icon_link_broken.png by tools/convert_image.py

// 13x14 INDEXED_1BIT — 36 bytes (28 pixel + 8 palette)
static const uint8_t icon_link_broken_data[] = {
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0xBE, 0x18, 0x9C, 0x08, 0xCC, 0xC8,
    0xE7, 0xC8, 0xF3, 0x88, 0xF9, 0x98, 0xCC, 0xF8, 0x8E, 0x68, 0x9F, 0x38, 0x99, 0x98, 0x81, 0x48,
    0xC3, 0x68, 0xFF, 0xF8,
};

const lv_img_dsc_t icon_link_broken = {
    .header = {
        .cf = LV_IMG_CF_INDEXED_1BIT,
        .always_zero = 0,
        .reserved = 0,
        .w = 13,
        .h = 14,
    },
    .data_size = sizeof(icon_link_broken_data),
    .data = icon_link_broken_data,
};

