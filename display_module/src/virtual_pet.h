#pragma once
#include <lvgl.h>

/*
 * virtual_pet — owns pet state, stat cycling, character count, and pet area rendering.
 *
 * Call virtual_pet_init() once from build_real_screen() on the central half,
 * passing widget shells created by custom_display.c. The pet module owns all
 * content within those widgets from that point on.
 *
 * virtual_pet_cycle_stat() is called by custom_display.c when &cycle fires in
 * CUSTOM display state.
 */

void virtual_pet_init(lv_obj_t *pet_container, lv_obj_t *status_icon, lv_obj_t *status_label);
void virtual_pet_cycle_stat(void);
