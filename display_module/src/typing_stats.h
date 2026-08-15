#pragma once
#include <lvgl.h>
#include <stdint.h>

void     typing_stats_init(lv_obj_t *status_icon, lv_obj_t *status_label);
void     typing_stats_cycle(void);
uint32_t typing_stats_get_char_count(void); /* total keypresses this session */
