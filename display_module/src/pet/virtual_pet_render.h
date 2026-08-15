/*
 * virtual_pet_render.h — Pet renderer public API.
 *
 * The renderer reads a pet_state_t and draws one frame onto an lv_canvas.
 * It knows nothing about pet logic — only how to draw it.
 *
 * HOW TO ADD SPRITES:
 *   1. Draw your frames and export as a PNG sprite sheet.
 *   2. Convert: python3 tools/convert_image.py sheet.png --sprite-w W --sprite-h H --names frame_0 frame_1 ...
 *   3. Include the generated .h in virtual_pet_render.c (already has a section for this).
 *   4. Add the frame pointers to the sprite table for that behavior.
 *   5. Update frame_count in anim_clips[] in virtual_pet_state.c to match.
 */

#pragma once
#include "virtual_pet_state.h"
#include <lvgl.h>

/*
 * Call once when the display is built.
 * canvas: an lv_canvas object covering the pet area (PET_AREA_WIDTH × PET_AREA_HEIGHT).
 */
void pet_render_init(lv_obj_t *canvas);

/*
 * Draw the current frame of the pet into the canvas.
 * Call every PET_RENDER_TICK_MS milliseconds, after pet_state_tick().
 */
void pet_render(const pet_state_t *pet);
