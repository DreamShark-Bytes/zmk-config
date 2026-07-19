/*
 * virtual_pet.c — stub. Pet engine not yet implemented.
 *
 * Character count and WPM are owned by typing_stats.c — import from there.
 * This file is preserved so the feature/virtual-pet branch starts from a
 * known-good foundation rather than having to restore from git history.
 */

#include "virtual_pet.h"
#include "display_config.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>

LOG_MODULE_REGISTER(virtual_pet, CONFIG_ZMK_LOG_LEVEL);

void virtual_pet_init(lv_obj_t *pet_container, lv_obj_t *status_icon,
                      lv_obj_t *status_label) {
    /* no-op until pet engine is implemented */
    (void)pet_container;
    (void)status_icon;
    (void)status_label;
}
