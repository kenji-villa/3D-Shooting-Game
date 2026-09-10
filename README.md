# Aim and Shoot

A 3D first-person shooting-range game built from scratch in C++17 with OpenGL 1.1, GLU, and freeglut. Aim with the mouse, fire bullets or lobbed grenades at a moving, distance-scaled bullseye, and rack up points before the round ends.

![Engine](https://img.shields.io/badge/engine-OpenGL%201.1%20%2B%20freeglut-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B17-00599C)
![Build](https://img.shields.io/badge/build-CMake-064F8C)
![Platform](https://img.shields.io/badge/platform-Windows%20(MinGW)-lightgrey)

---

## Features

- **First-person aiming** — full mouse-look (yaw + pitch, clamped), with an on-screen scope reticle
- **Two weapons** — a fast, flat-trajectory bullet and a gravity-arced grenade, switchable mid-round
- **5-ring bullseye scoring** — white / purple / blue / red / yellow rings worth 1–5 points, matched exactly to hit detection
- **Distance-aware target sizing** — targets scale up the farther away they spawn, so long-range shots stay fair
- **Randomized target respawn** — a new position (and size) every time you score
- **Win condition** — first to 20 points triggers a Game Over screen with a "Play Again" prompt
- **Textured environment** — tiled floor and wall textures loaded from PNG files
- **Sound effects** — a real gunshot SFX on every shot, via a lightweight audio engine
- **A properly modeled bullet** — brass casing + copper nose, oriented to actually point where it's flying


## Controls

| Input | Action |
|---|---|
| Mouse | Aim (look/turn) |
| Left Click / `Space` | Fire |
| `1` | Select bullet |
| `2` | Select grenade |
| `N` | New round (mid-game) / Play Again (after Game Over) |
| `Esc` | Quit |

## How It Works

The game runs on a small state machine — **Aiming → Firing → HitStop → Aiming**, looping until the score hits 20, at which point it drops into **GameOver** until the player restarts. Full breakdown of the game loop, every module, and the asset pipeline is in [`DOCUMENTATION.md`](DOCUMENTATION.md).

## Project Structure

```
.
├── CMakeLists.txt
├── assets/                # Textures and audio, copied next to the built exe on every build
│   ├── Brick_02.png       # Floor texture
│   ├── Brick_03.png       # Wall texture
│   └── cz_trimmed.wav     # Gunshot sound effect
├── third_party/           # Vendored single-header libraries
│   ├── stb_image.h        # PNG/JPG loading
│   └── miniaudio.h        # Audio playback
├── src/
│   ├── main.cpp            # Entry point, freeglut callback wiring, startup
│   ├── Globals.h           # extern declarations for shared game state
│   ├── Types.h              # Vec3 math primitives
│   ├── Camera.h / .cpp       # First-person aiming camera
│   ├── Room.h / .cpp          # Static room geometry (floor + walls)
│   ├── Target.h / .cpp         # Bullseye: spawning, drawing, scoring
│   ├── Projectile.h / .cpp      # Bullet/grenade physics and models
│   ├── GameState.h / .cpp        # The Aiming/Firing/HitStop/GameOver state machine
│   ├── Renderer.h / .cpp          # 3D scene draw orchestration
│   ├── Hud.h / .cpp                # 2D overlay: score, scope, Game Over screen
│   ├── Input.h / .cpp               # Mouse/keyboard callbacks
│   ├── Texture.h / .cpp              # PNG loading → OpenGL textures (via stb_image)
│   └── Audio.h / .cpp                 # Sound playback (via miniaudio)
└── DOCUMENTATION.md        # Full technical write-up of every module and function
```

## Prerequisites

- **[freeglut 3.2.1](https://freeglut.sourceforge.net/)** — built/installed locally (this project expects it at `C:/freeglut-3.2.1/install`; adjust `FREEGLUT_ROOT` in `CMakeLists.txt` if yours lives elsewhere)
- **CMake 3.16+**
- **MinGW-w64** (via [MSYS2](https://www.msys2.org/)) — the project is built and tested against MinGW's `g++`/`gdb`
- **Ninja** (or another CMake generator of your choice — the included VS Code tasks assume Ninja)

## Building

### With VS Code

The repo includes `.vscode/tasks.json` and `.vscode/launch.json` for a one-click workflow:

1. Run **CMake Configure** (generates the build directory with Ninja)
2. Run **CMake Build** (compiles the project — this is also the default build task, `Ctrl+Shift+B`)
3. Press **F5** to launch with the debugger attached (`gdb`)

### From the command line

```sh
cmake -S . -B build -G Ninja
cmake --build build
./build/ShootingGame.exe
```

The build automatically copies `libfreeglut.dll` and the entire `assets/` folder next to the built executable — no manual file copying needed after a build.

## Dependencies

| Library | Purpose | License |
|---|---|---|
| [freeglut](https://freeglut.sourceforge.net/) | Windowing, input, OpenGL context | MIT |
| OpenGL / GLU | 3D rendering | — |
| [stb_image](https://github.com/nothings/stb) | PNG texture loading | Public domain / MIT-0 |
| [miniaudio](https://github.com/mackron/miniaudio) | Audio playback | Public domain / MIT-0 |

`stb_image.h` and `miniaudio.h` are vendored directly in `third_party/` as single-header libraries — no separate installation step required.

## Assets

Textures and audio credit their original sources under their respective licenses (see individual asset pages if sourced from OpenGameArt.org or similar). If you swap in your own assets, drop them in `assets/` and load them via `texture_load()` / `audio_play_sound()` — the whole folder is copied automatically on every build, so no `CMakeLists.txt` changes are needed.

## Roadmap

Ideas for future iterations, not yet implemented:

- [ ] A visible pistol model held in the player's hand, with recoil animation
- [ ] Difficulty levels with moving/patrolling targets
- [ ] Per-level environment themes (different wall/floor textures, room layouts)
- [ ] Mipmapped texture filtering (currently bilinear-only — see `DOCUMENTATION.md` for why)

