
# Wind Waker HD AutoTracker

A guest mod for The Legend of Zelda: The Wind Waker HD Recompiled.

## Purpose

The AutoTracker reads live game state and makes it available to the existing WWHD AutoTracker application.

The intended integration is:

1. Read inventory and relevant game state from the running game.
2. Write the state to the mod's private data directory.
3. Allow the Python AutoTracker application to consume that data.
4. Update the tracker using the existing spoiler and location logic.

## Current development status

- [x] Guest mod loads in game.
- [x] HUD connection message displays.
- [x] Test JSON file can be written.
- [ ] Read and verify live inventory data.
- [ ] Identify the active randomizer seed.
- [ ] Identify completed randomized checks.
- [ ] Connect the game data to the Python tracker.

## Building

The mod uses the Wind Waker HD Guest Mod SDK.

The repository catalogue build process compiles `src/mod.c` into a PowerPC guest module.

## Compatibility

Designed for ZeldaWWHDRecomp guest mod support.

This project is experimental and is not yet considered a verified release.

