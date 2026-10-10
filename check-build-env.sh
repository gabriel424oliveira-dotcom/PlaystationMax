#!/usr/bin/env bash
set -u
printf 'PlaystationMax build environment check\n\n'
missing=0
for tool in cmake make clang++ python3; do
  if command -v "$tool" >/dev/null 2>&1; then printf '[OK] %s: %s\n' "$tool" "$(command -v "$tool")"; else printf '[FALTA] %s\n' "$tool"; missing=1; fi
done
printf '\nSDL dependencies (desktop test):\n'
if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists sdl2 SDL2_ttf; then
  printf '[OK] SDL2 %s; SDL2_ttf %s\n' "$(pkg-config --modversion sdl2)" "$(pkg-config --modversion SDL2_ttf)"
else
  printf '[FALTA] SDL2 and/or SDL2_ttf development packages\n'
  missing=1
fi
printf '\nPS4 toolchain/package tools:\n'
found=0
for tool in orbis-clang orbis-clang++ create-fself create-gp4 pkgbuild; do
  if command -v "$tool" >/dev/null 2>&1; then printf '[OK] %s: %s\n' "$tool" "$(command -v "$tool")"; found=1; fi
done
if [ "$found" -eq 0 ]; then
  printf '[FALTA] No recognized OpenOrbis/PS4 packaging commands found in PATH.\n'
  missing=1
fi
printf '\nImportant: this check does not prove PS4 firmware/GoldHEN compatibility.\n'
if [ "$missing" -eq 0 ]; then printf '\nBasic prerequisites found; a PS4 build still needs the matching sample/build files.\n'; else printf '\nEnvironment is not ready to produce a tested PS4 PKG.\n'; fi
exit 0
