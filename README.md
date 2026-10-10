# PlaystationMax v0.3 — interface prototype

This version adds a **native C++/SDL2 interface prototype** for the PS4 homebrew project, plus a catalogue sample and Android-friendly build notes.

## What works in the source prototype
- Full-screen SDL2 window
- Home screen with categories and poster-style cards
- Keyboard navigation for desktop testing
- SDL game-controller navigation where supported by the chosen SDL-PS4 port
- Category selection and a selected-item details panel

## What is not implemented yet
- Video decoding/playback
- USB filesystem discovery
- HTTP media fetching
- Cover-image loading
- Persistent favorites/settings
- A verified PS4 build configuration or a tested `.pkg`

## Your environment
You said you have only an Android phone, PS4 system software 13.52, and GoldHEN. I cannot verify from those details alone that the specific GoldHEN build and PS4 firmware are compatible with a particular OpenOrbis package. Confirm compatibility with the source/toolchain version and the homebrew environment you already use.

## Build approach
Use the official OpenOrbis toolchain and a matching SDL-PS4 port:
- Toolchain: https://github.com/OpenOrbis/OpenOrbis-PS4-Toolchain
- SDL-PS4: https://github.com/OpenOrbis/SDL-PS4

Do not compile this as an ordinary desktop SDL app and assume the output will run on PS4. Start from the sample and build files included with the exact SDL-PS4/toolchain setup, then transplant `src/main.cpp` and add the SDL_ttf dependency only if it is built for that same target. If SDL_ttf is unavailable in your environment, port the labels to the sample's available font/text renderer.

The source is intentionally a UI milestone only. It does not claim to produce a ready-to-install PKG.
