# CS2 External ESP Demo (C++)

A standalone Windows overlay demonstrating ESP-style drawing with **synthetic test targets only**. It does not read CS2 memory, inspect game files, inject into the game, or reveal real player locations.

## Requirements
- Windows 10 or 11
- Visual Studio 2022
- Workload: **Desktop development with C++**
- MSVC v143 toolset and a Windows 10/11 SDK

## Build
1. Clone or download this repository.
2. Open `CS2_External_ESP_Demo.vcxproj` in Visual Studio.
3. Select `x64` and `Release`.
4. Choose **Build → Build Solution**.
5. Run the generated `CS2_External_ESP_Demo.exe`.

## Controls
- `F1`: Toggle the synthetic ESP drawings on/off.
- `F2`: Close the overlay.

The overlay is topmost, click-through, and uses a black color key for transparency. The demo targets move around the screen to let you test boxes, labels, and health bars without interacting with CS2.

## Troubleshooting
- If Visual Studio asks to retarget the project, choose the installed Windows SDK.
- If the project cannot find the compiler, install the **Desktop development with C++** workload from Visual Studio Installer.
- If Windows blocks the executable, build it locally from the source rather than downloading a prebuilt binary.
