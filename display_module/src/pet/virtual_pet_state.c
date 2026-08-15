/*
 * virtual_pet_state.c — Pet state machine: tick logic and transitions.
 *
 * This file has NO dependency on LVGL or ZMK. It is pure portable C that
 * can run in a PC simulator without modification.
 *
 * ─── HOW THINGS CONNECT ─────────────────────────────────────────────────────
 *
 *   pet_config.h        ← tune all timing, thresholds, world size here
 *   virtual_pet_state.c ← the logic (this file)
 *   virtual_pet_render.c ← reads pet_state_t and draws it on the LVGL canvas
 *   custom_display.c    ← owns the pet_state_t global, drives the tick timer
 *
 * ─── HOW TO ADD A BEHAVIOR ──────────────────────────────────────────────────
 *
 *   1. Add the name to pet_behavior_t in virtual_pet_state.h
 *   2. Add a row to anim_clips[] below (frame count, speed, loop mode)
 *   3. Add a sprite table entry in virtual_pet_render.c
 *   4. Add transition logic in pet_state_tick() if needed
 *
 * ─── ANIMATION CLIP TABLE ───────────────────────────────────────────────────
 *
 * Each behavior has one "clip": how many frames it has, how fast they play,
 * and whether the clip loops or plays once and stops.
 *
 * Frame counts are PLACEHOLDERS until the real sprite sheets are done.
 * Update frame_count here when you add frames to virtual_pet_render.c.
 */

#include "virtual_pet_state.h"
#include "pet_config.h"  /* all pet timing/world constants — no display dependency */

#include <string.h>   /* memset */
#include <stdbool.h>


// ─── Animation clip definitions ──────────────────────────────────────────────

typedef struct {
    uint16_t frame_interval_ms;  // how long each frame shows (ms)
    uint8_t  frame_count;        // total frames in this clip
    bool     loop;               // true = loop forever; false = play once and hold
} pet_clip_def_t;

/*
 * One entry per pet_behavior_t. Index must match the enum value exactly.
 *
 * frame_count is the number of sprite frames you have drawn for this behavior.
 * Update it here when you finish drawing the art — the renderer uses the same
 * count via pet_render_frame_count() so both stay in sync automatically.
 */
static const pet_clip_def_t anim_clips[PET_BEHAVIOR_COUNT] = {
    /* behavior                frame_ms  frames  loop  */
    [PET_BEHAVIOR_EGG_IDLE]  = { 800,    2,     true  },  /* slow bob */
    [PET_BEHAVIOR_HATCHING]  = { 120,    2,     false },  /* fast crack, plays once — UPDATE frame count when art is done */
    [PET_BEHAVIOR_IDLE]      = { 400,    2,     true  },  /* gentle breathing — UPDATE frame count */
    [PET_BEHAVIOR_WALK_LEFT] = { 200,    2,     true  },  /* walking legs — UPDATE frame count */
    [PET_BEHAVIOR_WALK_RIGHT]= { 200,    2,     true  },  /* same frames, flipped by renderer */
    [PET_BEHAVIOR_REACT]     = {  80,    2,     false },  /* quick bounce, plays once — UPDATE frame count */
    [PET_BEHAVIOR_FEED]      = { 300,    2,     false },  /* eating, plays once — UPDATE frame count */
};


// ─── Helpers ─────────────────────────────────────────────────────────────────

/*
 * Returns how many frames the current behavior's clip has.
 * The renderer calls this so both agree on frame count without duplicating data.
 */
uint8_t pet_render_frame_count(pet_behavior_t behavior) {
    if (behavior >= PET_BEHAVIOR_COUNT) return 1;
    return anim_clips[behavior].frame_count;
}

/*
 * Switch the pet to a new behavior and restart the animation from frame 0.
 * Does nothing if the pet is already in that behavior (no flicker).
 */
static void set_behavior(pet_state_t *pet, pet_behavior_t behavior) {
    if (pet->behavior == behavior) return;
    pet->behavior        = behavior;
    pet->anim_frame      = 0;
    pet->anim_elapsed_ms = 0;
    pet->anim_done       = false;
}

/*
 * Clamp a value to [lo, hi].
 */
static int16_t clamp16(int16_t v, int16_t lo, int16_t hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static uint8_t clamp8(int v, int lo, int hi) {
    if (v < lo) return (uint8_t)lo;
    if (v > hi) return (uint8_t)hi;
    return (uint8_t)v;
}


// ─── Tick sub-systems ────────────────────────────────────────────────────────

/*
 * Advance the animation one tick.
 * When a looping clip reaches the last frame it wraps to 0.
 * When a one-shot clip reaches the last frame it sets anim_done = true
 * and holds the last frame until the outer logic transitions state.
 */
static void tick_animation(pet_state_t *pet, uint32_t dt_ms) {
    const pet_clip_def_t *clip = &anim_clips[pet->behavior];
    if (clip->frame_count == 0 || pet->anim_done) return;

    pet->anim_elapsed_ms += dt_ms;
    if (pet->anim_elapsed_ms < clip->frame_interval_ms) return;

    pet->anim_elapsed_ms = 0;

    uint8_t next = pet->anim_frame + 1;
    if (next < clip->frame_count) {
        pet->anim_frame = next;
    } else if (clip->loop) {
        pet->anim_frame = 0;         /* loop back to start */
    } else {
        pet->anim_done = true;       /* hold last frame; caller handles transition */
    }
}

/*
 * Move the pet one step toward its target and recompute the background scroll.
 *
 * Background offset formula:
 *   ideal  = world_x - viewport_half        (keep pet centered)
 *   offset = clamp(ideal, 0, world_max)     (don't scroll past world edges)
 *   screen_x = world_x - offset             (what the renderer uses to place the sprite)
 *
 * At world edges the pet walks to the visible edge of the screen instead of
 * the background scrolling further — a natural "wall" feel.
 */
static void tick_spatial(pet_state_t *pet) {
    if (pet->spatial.speed == 0) return;

    int16_t dx = pet->spatial.target_x - pet->spatial.world_x;

    if (dx > pet->spatial.speed) {
        pet->spatial.world_x += pet->spatial.speed;
    } else if (dx < -pet->spatial.speed) {
        pet->spatial.world_x -= pet->spatial.speed;
    } else {
        pet->spatial.world_x = pet->spatial.target_x;  /* arrived */
    }

    /* Clamp inside world bounds */
    pet->spatial.world_x = clamp16(pet->spatial.world_x, 0, PET_WORLD_WIDTH);

    /* Recompute background scroll offset */
    int16_t half       = PET_CANVAS_W / 2;
    int16_t ideal      = pet->spatial.world_x - half;
    int16_t max_offset = PET_WORLD_WIDTH - PET_CANVAS_W;
    pet->spatial.bg_offset = clamp16(ideal, 0, max_offset);
}

/* tick_plant() — removed pending design of the composite body-parts system.
 * See PLANNED.md → "Composite body parts" for the planned approach. */

/*
 * Decay hunger and happiness once every PET_STAT_DECAY_TICKS render ticks.
 * Values are clamped to 0 (they don't go negative).
 */
static void tick_stat_decay(pet_state_t *pet) {
    if ((pet->stats.age_ticks % PET_STAT_DECAY_TICKS) != 0) return;
    pet->stats.hunger    = clamp8((int)pet->stats.hunger    - PET_HUNGER_DECAY,    0, 100);
    pet->stats.happiness = clamp8((int)pet->stats.happiness - PET_HAPPINESS_DECAY, 0, 100);
}

/*
 * Decide whether an idle pet should start wandering.
 * Called every tick; acts only on the wander interval boundary.
 *
 * Uses age_ticks as a simple alternating signal — no random number generator
 * needed, and the pet reliably moves both directions over time.
 * Replace this logic later if you want truly random wandering.
 */
static void tick_idle_wander(pet_state_t *pet) {
    if (pet->behavior != PET_BEHAVIOR_IDLE) return;
    if ((pet->stats.age_ticks % PET_WANDER_INTERVAL_TICKS) != 0) return;

    uint32_t cycle = pet->stats.age_ticks / PET_WANDER_INTERVAL_TICKS;
    if (cycle % 2 == 0) {
        /* Walk toward left quarter of the world */
        pet->spatial.target_x = PET_WORLD_WIDTH / 4;
        pet->spatial.speed    = PET_WALK_SPEED_PX;
        pet->facing           = PET_FACE_LEFT;
        set_behavior(pet, PET_BEHAVIOR_WALK_LEFT);
    } else {
        /* Walk toward right three-quarter of the world */
        pet->spatial.target_x = (PET_WORLD_WIDTH * 3) / 4;
        pet->spatial.speed    = PET_WALK_SPEED_PX;
        pet->facing           = PET_FACE_RIGHT;
        set_behavior(pet, PET_BEHAVIOR_WALK_RIGHT);
    }
}


// ─── Public API ──────────────────────────────────────────────────────────────

void pet_state_init(pet_state_t *pet) {
    memset(pet, 0, sizeof(*pet));

    pet->phase    = PET_PHASE_EGG;
    pet->behavior = PET_BEHAVIOR_EGG_IDLE;
    pet->facing   = PET_FACE_RIGHT;

    /* Start with comfortable stats */
    pet->stats.hunger    = 80;
    pet->stats.happiness = 80;

    /* Start centered in the world so the pet can wander both ways */
    pet->spatial.world_x  = PET_WORLD_WIDTH / 2;
    pet->spatial.target_x = PET_WORLD_WIDTH / 2;
}

/*
 * Main tick — call this every PET_RENDER_TICK_MS milliseconds.
 *
 * dt_ms:    time since the last tick in milliseconds (usually PET_RENDER_TICK_MS)
 * keycount: current total from typing_stats_get_char_count()
 */
void pet_state_tick(pet_state_t *pet, uint32_t dt_ms, uint32_t keycount) {
    pet->stats.age_ticks++;
    pet->stats.xp = keycount;  /* mirror keycount into XP; the renderer can show it */

    switch (pet->phase) {

    // ── EGG ──────────────────────────────────────────────────────────────────
    case PET_PHASE_EGG:
        tick_animation(pet, dt_ms);

        /* Hatch when the egg has been on-screen long enough */
        if ((uint64_t)pet->stats.age_ticks * PET_RENDER_TICK_MS >= PET_EGG_HATCH_MS) {
            pet->phase    = PET_PHASE_HATCHING;
            set_behavior(pet, PET_BEHAVIOR_HATCHING);
        }
        break;

    // ── HATCHING ─────────────────────────────────────────────────────────────
    case PET_PHASE_HATCHING:
        tick_animation(pet, dt_ms);

        /* When the hatch animation finishes, transition to alive */
        if (pet->anim_done) {
            pet->phase    = PET_PHASE_ALIVE;
            set_behavior(pet, PET_BEHAVIOR_IDLE);
        }
        break;

    // ── ALIVE — normal play loop ──────────────────────────────────────────────
    case PET_PHASE_ALIVE:
        tick_animation(pet, dt_ms);
        tick_spatial(pet);
        tick_stat_decay(pet);

        /* One-shot animations (REACT, FEED, HATCHING) return to IDLE when done */
        if (pet->anim_done) {
            pet->spatial.speed = 0;
            set_behavior(pet, PET_BEHAVIOR_IDLE);
        }

        /* Walking pet arrived at its target — return to idle */
        if ((pet->behavior == PET_BEHAVIOR_WALK_LEFT ||
             pet->behavior == PET_BEHAVIOR_WALK_RIGHT) &&
            pet->spatial.world_x == pet->spatial.target_x) {
            pet->spatial.speed = 0;
            set_behavior(pet, PET_BEHAVIOR_IDLE);
        }

        /* Decide whether to start wandering */
        tick_idle_wander(pet);
        break;

    default:
        break;
    }
}
