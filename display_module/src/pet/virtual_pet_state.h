/*
 * virtual_pet_state.h — Pet state machine: structs, enums, and public API.
 *
 * This file has NO dependency on LVGL or ZMK — it is pure portable C so it
 * can run unchanged in a PC simulator. All display work lives in
 * virtual_pet_render.c, which reads from pet_state_t.
 *
 * HOW TO EXTEND:
 *   - New behavior (e.g. SLEEP): add to pet_behavior_t, add a row to
 *     anim_clips[] in virtual_pet_state.c, handle in pet_state_tick().
 *   - New evolution stage: add to pet_phase_t; duplicate the ALIVE case
 *     in pet_state_tick() and tune as needed.
 *   - Tune numbers without touching code: edit display_config.h.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>


// ─── Lifecycle phase ─────────────────────────────────────────────────────────
// The pet's overall stage of life. Each phase drives a different branch of
// the tick logic and a different set of allowed behaviors.

typedef enum {
    PET_PHASE_EGG      = 0,  // Sitting as an egg, waiting to hatch
    PET_PHASE_HATCHING,       // Playing the one-shot hatch animation
    PET_PHASE_ALIVE,          // Fully hatched — the normal play loop
    PET_PHASE_EVOLVING,       // Playing an evolution animation (future)
    PET_PHASE_COUNT,
} pet_phase_t;


// ─── Behavior state ───────────────────────────────────────────────────────────
// What the pet is actively doing right now. Each behavior maps to one
// animation clip (defined in virtual_pet_state.c's anim_clips[]).

typedef enum {
    PET_BEHAVIOR_EGG_IDLE   = 0,  // Egg bobbing gently
    PET_BEHAVIOR_HATCHING,         // Egg cracking open (one-shot)
    PET_BEHAVIOR_IDLE,             // Keykey standing, breathing
    PET_BEHAVIOR_WALK_LEFT,        // Keykey walking left
    PET_BEHAVIOR_WALK_RIGHT,       // Keykey walking right
    PET_BEHAVIOR_REACT,            // Quick reaction (e.g. burst typing)
    PET_BEHAVIOR_FEED,             // Eating something (one-shot)
    PET_BEHAVIOR_COUNT,
} pet_behavior_t;


// ─── Facing direction ─────────────────────────────────────────────────────────
// Sprites are drawn facing right. Facing left = horizontal flip at render time.
// See virtual_pet_render.c → blit_sprite().

typedef enum {
    PET_FACE_RIGHT = 0,
    PET_FACE_LEFT,
} pet_facing_t;


// ─── Stats ────────────────────────────────────────────────────────────────────
// The pet's needs and growth. All values 0–100 unless noted.
// Decay rates and thresholds are set in display_config.h.

typedef struct {
    uint8_t  hunger;      // 100 = full, 0 = starving
    uint8_t  happiness;   // 100 = ecstatic, 0 = miserable
    uint32_t xp;          // lifetime keypresses (used for evolution path)
    uint32_t age_ticks;   // render ticks since last boot; resets on power-off (Digimon-style)
} pet_stats_t;


// ─── Spatial state ────────────────────────────────────────────────────────────
// The pet's position in world-space and derived background scroll offset.
// World is PET_WORLD_WIDTH pixels wide; the viewport is PET_CANVAS_W.
// bg_offset is recomputed from world_x every tick — do not set it directly.

typedef struct {
    int16_t world_x;    // pet's x in world-space pixels
    int16_t target_x;   // where the pet is walking toward (world-space)
    int16_t bg_offset;  // how far the background has scrolled (read-only for render)
    int8_t  speed;      // pixels per render tick while walking (0 = stopped)
} pet_spatial_t;


// ─── Composite body parts (future design) ────────────────────────────────────
// Planned: separate sprite layers for eyes, mouth, accessories, etc. so parts
// can animate independently of the body (blink in any body frame, swap eye
// types, open/close mouth mid-walk, etc.). Not all pets will use all layers —
// a pet that is just a giant mouth would skip the eye layer entirely.
// Design this before implementing — see PLANNED.md → "Composite body parts".


// ─── Full pet state ───────────────────────────────────────────────────────────
// Everything the renderer needs to draw one frame. The state machine owns and
// mutates this struct; the renderer only reads it.

typedef struct {
    pet_phase_t    phase;
    pet_behavior_t behavior;
    pet_facing_t   facing;
    pet_stats_t    stats;
    pet_spatial_t  spatial;

    // Animation playback — managed by the state machine, read by the renderer
    uint8_t  anim_frame;        // index of the current frame within the clip
    uint32_t anim_elapsed_ms;   // milliseconds since this frame started
    bool     anim_done;         // true when a one-shot clip has played fully

    // Composite body-part state goes here once designed
    // e.g. uint8_t eye_frame; uint8_t mouth_frame; pet_expression_t expression;
} pet_state_t;


// ─── Public API ──────────────────────────────────────────────────────────────

/* Set up a fresh pet (called once at startup or on reset). */
void pet_state_init(pet_state_t *pet);

/*
 * Advance the state machine by dt_ms milliseconds.
 * Call this every PET_RENDER_TICK_MS milliseconds from the display work queue.
 * Pass the current total keypress count so the pet gains XP.
 */
void pet_state_tick(pet_state_t *pet, uint32_t dt_ms, uint32_t keycount);

/*
 * Returns the number of sprite frames for a given behavior.
 * The renderer calls this so frame count is defined once (in anim_clips[])
 * and shared — no duplication between state machine and renderer.
 */
uint8_t pet_render_frame_count(pet_behavior_t behavior);
