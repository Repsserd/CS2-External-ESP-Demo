# Nexus Game Hub — ExLoader-inspired starter

A small Windows C++ launcher UI inspired by game-library/loader applications.

## Features
- Dark Windows desktop interface
- Game library entry for Counter-Strike 2
- Local demo plugin list with add/remove UI
- Activity/status panel
- CMake project for Visual Studio 2022

## Build
1. Install Visual Studio 2022 with **Desktop development with C++**.
2. Open this folder in Visual Studio, or run:
   ```powershell
   cmake -S . -B build
   cmake --build build --config Release
   ```
3. Run `build/Release/NexusGameHub.exe`.

## Safety and scope
This is a benign launcher UI starter. It does not inject DLLs, read game memory,
modify CS2, bypass anti-cheat, or provide cheat functionality. The plugin list
is illustrative and does not load executable plugins.
