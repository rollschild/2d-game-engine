# 2D Game Engine

[![built with nix](https://builtwithnix.org/badge.svg)](https://builtwithnix.org)

A simple but feature-rich game engine written in C++, with Lua scripting capabilities.

Status: FINISHED.

## Build & Run

### Build

- `nix develop` to enable Nix
- `cmake . -B build`
- `cmake --build build`

### Run

- `./build/src/main`
- Use arrow keys to control the main player
- Use `<space>` key to shoot bullets
- Press `d` key to enable/disable troubleshooting mode

## Dependencies

- `SDL`
  - for multi-platform capability
- [Dear ImGui](https://github.com/ocornut/imgui)
  - for troubleshooting GUI
- [sol](https://github.com/ThePhD/sol2)
  - for Lua and C++ bindings
