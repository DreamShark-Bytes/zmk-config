#pragma once
#include <lvgl.h>
// Generated from resources/icons/icon_link_broken.png by tools/convert_image.py

// 13x14 INDEXED_1BIT — 36 bytes (28 pixel + 8 palette)
static const uint8_t icon_link_broken_data[] = {
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x41, 0xE0, 0x63, 0xF0, 0x33, 0x30,
    0x18, 0x30, 0x0C, 0x70, 0x06, 0x60, 0x33, 0x00, 0x71, 0x90, 0x60, 0xC0, 0x66, 0x60, 0x7E, 0xB0,
    0x3C, 0x90, 0x00, 0x00,
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

