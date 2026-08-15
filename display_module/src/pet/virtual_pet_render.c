/*
 * virtual_pet_render.c — Pet renderer: canvas blit, sprite tables, background.
 *
 * ─── HOW TO ADD YOUR SPRITE FRAMES ─────────────────────────────────────────
 *
 *   1. Draw all states for one creature in Pixquare/Procreate.
 *      Lay them out as a sprite sheet — one row per animation, left to right.
 *      Name the file after the creature: keykey.png, egg.png, etc.
 *
 *   2. Convert — the filename becomes the C variable prefix automatically:
 *        python3 tools/convert_image.py resources/pet/keykey.png \
 *            --sprite-w 62 --sprite-h 62 \
 *            --groups idle 2 walk 3 eat 2
 *      Writes resources/pet/keykey.h containing:
 *        keykey_idle_0, keykey_idle_1          (0-indexed frame variables)
 *        keykey_walk_0 ... keykey_walk_2
 *        keykey_idle_frames[], keykey_idle_count   (plug into sprite_table[])
 *        keykey_walk_frames[], keykey_walk_count
 *
 *   3. Add the include in the SPRITE INCLUDES section below:
 *        #include "keykey.h"
 *
 *   4. Replace placeholder arrays in sprite_table[] with the real ones:
 *        [PET_BEHAVIOR_IDLE]       = keykey_idle_frames,
 *        [PET_BEHAVIOR_WALK_LEFT]  = keykey_walk_frames,
 *        [PET_BEHAVIOR_WALK_RIGHT] = keykey_walk_frames,  // renderer flips it
 *
 *   5. Update frame_count in anim_clips[] in virtual_pet_state.c:
 *        [PET_BEHAVIOR_IDLE] = { .frame_interval_ms=400, .frame_count=keykey_idle_count, .loop=true },
 *
 * ─── SPRITE FORMAT ──────────────────────────────────────────────────────────
 *
 *   All sprites must be 62×62 pixels, 1-bit monochrome, INDEXED_1BIT format.
 *   The convert_image.py tool handles the format conversion automatically.
 *
 * ─── BACKGROUND ─────────────────────────────────────────────────────────────
 *
 *   draw_background() is called first each frame before the pet is blitted.
 *   Currently it fills the canvas white (blank). Replace it with a tileable
 *   background image when art is ready — the bg_offset from pet_state_t
 *   controls how far the background has scrolled.
 */

#include "virtual_pet_render.h"
#include "virtual_pet_state.h"
#include "display_config.h"

#include <lvgl.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(virtual_pet_render, CONFIG_ZMK_LOG_LEVEL);

// =============================================================================
// SPRITE INCLUDES
// Add your converted sprite headers here as you finish the art.
// =============================================================================

/* Placeholder frames — the egg bob images used until real Keykey art is ready. */
#include "pet_temp_image_1.h"
#include "pet_temp_image_2.h"

/* When you have real sprites, add lines like:
 *   #include "keykey_idle.h"     <- contains idle_0, idle_1, ...
 *   #include "keykey_walk.h"     <- contains walk_left_0, walk_left_1, ...
 *   #include "keykey_plant.h"    <- contains plant (single sprite)
 */


// =============================================================================
// SPRITE TABLES
// One entry per pet_behavior_t. Each entry is an array of frame pointers.
//
// To use placeholders for behaviors without art yet, just point them to the
// egg frames — the pet will show the egg bob for those states temporarily.
// =============================================================================

static const lv_img_dsc_t *frames_egg[]   = { &pet_temp_image_1, &pet_temp_image_2 };
static const lv_img_dsc_t *frames_idle[]  = { &pet_temp_image_1, &pet_temp_image_2 }; /* replace when Keykey idle art is done */
static const lv_img_dsc_t *frames_walk[]  = { &pet_temp_image_1, &pet_temp_image_2 }; /* replace when walk art is done */
static const lv_img_dsc_t *frames_react[] = { &pet_temp_image_1, &pet_temp_image_2 }; /* replace when react art is done */
static const lv_img_dsc_t *frames_feed[]  = { &pet_temp_image_1, &pet_temp_image_2 }; /* replace when feed art is done */

/*
 * sprite_table[behavior] → pointer to that behavior's frame array.
 * Index must match pet_behavior_t exactly.
 */
static const lv_img_dsc_t * const *sprite_table[PET_BEHAVIOR_COUNT] = {
    [PET_BEHAVIOR_EGG_IDLE]   = frames_egg,
    [PET_BEHAVIOR_HATCHING]   = frames_egg,   /* placeholder */
    [PET_BEHAVIOR_IDLE]       = frames_idle,
    [PET_BEHAVIOR_WALK_LEFT]  = frames_walk,
    [PET_BEHAVIOR_WALK_RIGHT] = frames_walk,  /* same frames, renderer flips them */
    [PET_BEHAVIOR_REACT]      = frames_react,
    [PET_BEHAVIOR_FEED]       = frames_feed,
};

/* Composite body-part sprites (eyes, mouth, accessories, etc.) go here once
 * the composite-parts system is designed. See PLANNED.md → "Composite body parts". */


// =============================================================================
// Canvas state
// =============================================================================

static lv_obj_t       *g_canvas     = NULL;
static lv_color_t      g_canvas_buf[PET_CANVAS_W * PET_CANVAS_H];


// =============================================================================
// Pixel blit helpers
// =============================================================================

/*
 * Read one pixel from an LVGL INDEXED_1BIT image at (x, y).
 * Returns 1 if the pixel is "white" (on), 0 if black (off).
 *
 * INDEXED_1BIT layout:
 *   bytes  0–7  : 2-colour palette (ignored at LV_COLOR_DEPTH=1)
 *   byte   8+   : packed pixel data, MSB first within each byte
 */
static int read_pixel(const lv_img_dsc_t *img, int x, int y) {
    if (x < 0 || x >= (int)img->header.w) return 0;
    if (y < 0 || y >= (int)img->header.h) return 0;

    const uint8_t *data  = img->data + 8;  /* skip palette */
    int            idx   = y * (int)img->header.w + x;
    uint8_t        byte  = data[idx / 8];
    return (byte >> (7 - (idx % 8))) & 1;
}

/*
 * Blit a sprite onto the canvas at screen position (dest_x, dest_y).
 *
 * flip_h = true: mirror the sprite horizontally (used for PET_FACE_LEFT).
 *
 * Pixels outside the canvas bounds are clipped automatically.
 * Transparent pixels (value 0 = black background in our palette convention)
 * are skipped so the background shows through — change the condition if your
 * art uses a different transparency convention.
 */
static void blit_sprite(const lv_img_dsc_t *sprite, int dest_x, int dest_y, bool flip_h) {
    if (!sprite || !g_canvas) return;

    int sw = (int)sprite->header.w;
    int sh = (int)sprite->header.h;

    for (int sy = 0; sy < sh; sy++) {
        for (int sx = 0; sx < sw; sx++) {
            /* When flipping, read from the mirror column */
            int src_x = flip_h ? (sw - 1 - sx) : sx;
            int pixel  = read_pixel(sprite, src_x, sy);

            /* Skip black (transparent) pixels — white pixels are the art */
            if (!pixel) continue;

            int cx = dest_x + sx;
            int cy = dest_y + sy;
            if (cx < 0 || cx >= PET_CANVAS_W)  continue;
            if (cy < 0 || cy >= PET_CANVAS_H) continue;

            lv_canvas_set_px(g_canvas, cx, cy, lv_color_white());
        }
    }
}


// =============================================================================
// Background rendering
// =============================================================================

/*
 * Draw the background for this frame.
 *
 * Currently fills the entire canvas black (blank) so sprite pixels stand out.
 * Replace this with a tiled background blit when background art is ready:
 *
 *   - bg_offset = pet->spatial.bg_offset (how far the background has scrolled)
 *   - Tile your background image starting at x = -bg_offset
 *   - The blit_sprite() function handles clipping at canvas edges
 *
 * Ground line example (uncomment to test the spatial model visually):
 *   lv_draw_line_dsc_t line_dsc;
 *   lv_draw_line_dsc_init(&line_dsc);
 *   line_dsc.color = lv_color_white();
 *   lv_canvas_draw_line(g_canvas, ...) -- see LVGL docs
 */
static void draw_background(const pet_state_t *pet) {
    /* Fill canvas black — every pixel starts off */
    lv_canvas_fill_bg(g_canvas, lv_color_black(), LV_OPA_COVER);

    /* BACKGROUND ART GOES HERE when you have it.
     * Use pet->spatial.bg_offset to offset the tile start position. */
    (void)pet;  /* suppress unused-parameter warning until background art exists */
}


// =============================================================================
// Public API
// =============================================================================

void pet_render_init(lv_obj_t *canvas) {
    g_canvas = canvas;
    lv_canvas_set_buffer(g_canvas, g_canvas_buf,
                         PET_CANVAS_W, PET_CANVAS_H,
                         LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(g_canvas, lv_color_black(), LV_OPA_COVER);
}

void pet_render(const pet_state_t *pet) {
    if (!g_canvas) return;

    /* 1. Clear and draw the background (scrolled by pet->spatial.bg_offset) */
    draw_background(pet);

    /* 2. Look up the current sprite frame for this behavior */
    pet_behavior_t beh = pet->behavior;
    if (beh >= PET_BEHAVIOR_COUNT || !sprite_table[beh]) return;

    uint8_t frame_count = pet_render_frame_count(beh);
    uint8_t frame_idx   = (pet->anim_frame < frame_count) ? pet->anim_frame : 0;
    const lv_img_dsc_t *sprite = sprite_table[beh][frame_idx];

    /* 3. Compute pet's screen x from its world position and background scroll */
    int screen_x = (int)pet->spatial.world_x - (int)pet->spatial.bg_offset;

    /* Center the sprite on screen_x (sprite is 62px wide, so offset by half) */
    int dest_x = screen_x - (int)sprite->header.w / 2;
    /* Vertically: sit the pet on a "ground" at the bottom of the canvas */
    int dest_y = PET_CANVAS_H - (int)sprite->header.h;

    /* 4. Blit the body, flipping horizontally if facing left */
    bool flip_h = (pet->facing == PET_FACE_LEFT);
    blit_sprite(sprite, dest_x, dest_y, flip_h);

    /* 5. Composite body parts (eyes, mouth, etc.) go here once designed.
     * See PLANNED.md → "Composite body parts". */
}
