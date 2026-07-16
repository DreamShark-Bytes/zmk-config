# zmk-config — Project Notes for Claude

Session rules (history format, STATUS.md, DESIGN.md policy) are in `~/.claude/CLAUDE.md`.

## Project files

| File | Role |
|---|---|
| `DESIGN.md` | Settled architecture, hardware/software specs, decision log |
| `STATUS.md` | Current firmware state, open items, current focus |
| `PLANNED.md` | Features in design — gitignored, not in repo |
| `zmk-config-Session-History.md` | Session log |
| `scratch.md` | Working scratchpad — gitignored |

## Build & flash workflow

Firmware is built by **GitHub Actions** on every push to any branch. There is no local build environment on the dev machine (Raspberry Pi 4B).

1. Edit code locally
2. Commit and push to the current branch (`new-feature--custom-display`)
3. GitHub Actions builds both halves and produces `.uf2` artifacts
4. Download `kyria_rev3_left-nice_nano_v2-zmk.uf2` and `kyria_rev3_right-nice_nano_v2-zmk.uf2`
5. Put each half into bootloader mode (double-tap reset), drag-and-drop `.uf2` onto the mounted drive

**GitHub Actions free tier:** 2,000 min/month for private repos. Each build is ~3–5 min per half, so roughly 500–600 builds before hitting the limit. Not a concern for normal iteration.

**Reading the memory report:** Actions tab → click the latest run → click either build job → expand "Run west build" → scroll to the end of that step. Zephyr prints flash and RAM usage there. Update the "Last measured" line in DESIGN.md → Resource estimates when the numbers change meaningfully.

## Where new code goes

| What | Where |
|---|---|
| Display layout / state machine | `display_module/src/custom_display.c` |
| User-tunable layout constants | `display_module/src/display_config.h` — edit here first |
| Demo advance behavior | `display_module/src/demo_cycle.c` |
| Keymap and layer definitions | `config/kyria_rev3.keymap` |
| Kconfig options | `config/kyria_rev3.conf` |
| Image/sprite conversion | `tools/convert_image.py` |
| Font build pipeline | `tools/build_font.sh` + `tools/png_to_icon_font.py` |
| Demo mockup source PNGs | `demos/` |
| Generated asset headers | `resources/` — do not edit manually |

## Commit workflow

Always write commit messages to `scratch.md` — never paste them into chat. See the global CLAUDE.md GitHub commits workflow for message format. Remind the user to push after committing.

Commit message body lines must NOT be wrapped at any fixed column width. The user commits through VSCode which has no column limit. Each bullet point is one unbroken line regardless of length.

## Architecture guardrails

- **LVGL calls only from the display work queue** — wrap in `k_work_submit_to_queue(zmk_display_work_q(), ...)`. Calling LVGL from any other context is unsafe.
- **`lv_img_set_src()` is a known crash** at `LV_COLOR_DEPTH=1` — do not re-introduce `lv_img` calls until the crash is isolated and resolved. Canvas pixel writes are the confirmed-safe path.
- **`display_config.h` is the user's control surface** — layout positions, font choices, layer names all live there. Never hardcode these values in `custom_display.c`.
- **No assets in `resources/fonts/generated/` or `resources/demos/`** should be hand-edited — they are tool outputs. Re-run the relevant script instead.
- **ZMK is not forked** — `display_module/` is a Zephyr extra module. Never patch ZMK source directly.

## Hardware & software specs

See `DESIGN.md` → "Hardware" section for full specs, limitations, and known LVGL constraints.
