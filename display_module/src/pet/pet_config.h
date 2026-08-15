/*
 * pet_config.h — All tunable constants for the virtual pet system.
 *
 * This is the only file you need to edit to tune pet timing, world size,
 * and stat rates. It is also the only file the pet modules depend on for
 * configuration, which makes the pet system easy to lift into another project:
 *
 *   LIFT-AND-SHIFT CHECKLIST
 *   Take these files:
 *     virtual_pet_state.c / virtual_pet_state.h
 *     virtual_pet_render.c / virtual_pet_render.h
 *     pet_config.h  ← this file
 *
 *   In the new project, define:
 *     PET_AREA_WIDTH   — width of the canvas the pet lives in (pixels)
 *     PET_AREA_HEIGHT  — height of that canvas (pixels)
 *   Either in this file directly or via your project's display config.
 *
 *   The render module needs LVGL (lv_canvas, lv_img_dsc_t).
 *   The state module has no external dependencies at all.
 */

#pragma once


// =============================================================================
// CANVAS SIZE
// How big is the area the pet lives in? For the Kyria rev3 left half this
// matches PET_AREA_WIDTH / PET_AREA_HEIGHT from display_config.h.
// If you lift this into another project, set these to your canvas size.
// =============================================================================
#ifndef PET_CANVAS_W
#define PET_CANVAS_W  62   /* same as PET_AREA_WIDTH in display_config.h */
#endif
#ifndef PET_CANVAS_H
#define PET_CANVAS_H  62   /* same as PET_AREA_HEIGHT in display_config.h */
#endif


// =============================================================================
// PET WORLD SIZE
// The pet inhabits a world wider than the canvas. The background scrolls to
// follow the pet, keeping it roughly centered. At world edges the pet walks
// to the canvas edge instead of the background scrolling further.
//
// PET_WORLD_WIDTH: total world-space width in pixels.
//   186 = 3 × canvas width — enough room to wander and back.
// =============================================================================
#define PET_WORLD_WIDTH  186


// =============================================================================
// TICK TIMING
// PET_RENDER_TICK_MS: how often the state machine advances and the canvas
//   redraws. 100 ms is smooth without being CPU-heavy.
// =============================================================================
#define PET_RENDER_TICK_MS  100   /* ms between pet ticks */


// =============================================================================
// EGG → HATCH
// How long (in milliseconds of on-time) the egg sits before hatching.
// Only counts while the keyboard is powered on — not wall-clock time.
// Set to 10000 (10 seconds) during testing; raise to 300000 (5 min) for real.
// =============================================================================
#define PET_EGG_HATCH_MS  300000   /* 5 minutes; reduce for testing */


// =============================================================================
// STAT DECAY
// Hunger and happiness drop slowly over time. Each decay event fires every
// PET_STAT_DECAY_TICKS render ticks (600 ticks × 100 ms = 60 seconds).
//
// PET_HUNGER_DECAY / PET_HAPPINESS_DECAY: points lost per decay event.
// Range is 0–100. Lower = slower starvation.
// =============================================================================
#define PET_STAT_DECAY_TICKS    600   /* ticks between decays (600 = 1 min) */
#define PET_HUNGER_DECAY          2   /* hunger lost per decay tick */
#define PET_HAPPINESS_DECAY       1   /* happiness lost per decay tick */


// =============================================================================
// MOVEMENT
// PET_WANDER_INTERVAL_TICKS: how many ticks between idle wander decisions.
//   300 ticks × 100 ms = 30 seconds between direction changes.
// PET_WALK_SPEED_PX: pixels per tick while walking. 1 px/tick at 100 ms/tick
//   = 10 px/sec — slow and deliberate.
// =============================================================================
#define PET_WANDER_INTERVAL_TICKS  300   /* ticks between wander decisions (30 s) */
#define PET_WALK_SPEED_PX            1   /* pixels per tick while walking */
