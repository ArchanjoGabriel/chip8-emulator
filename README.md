# CHIP-8 Emulator

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![CMake](https://img.shields.io/badge/CMake-3.28%2B-informational.svg)](https://cmake.org/)
[![SFML](https://img.shields.io/badge/SFML-3.1.0-brightgreen.svg)](https://www.sfml-dev.org/)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)](#)

A high-performance, cross-platform **CHIP-8 virtual machine & interpreter** written from scratch in modern **C++20**. Designed as a systems programming and computer architecture portfolio project, it features cycle-accurate execution, a complete 34-instruction opcode set, XOR-based graphics rendering via **SFML 3**, responsive 16-key input mapping, and hardware timers.

---

## Table of Contents

- [Overview](#overview)
- [Key Features](#key-features)
- [Virtual Machine Architecture](#virtual-machine-architecture)
- [Implemented Instruction Set](#implemented-instruction-set)
- [Keypad Mapping](#keypad-mapping)
- [Technical Implementation Details](#technical-implementation-details)
- [Project Structure](#project-structure)
- [Prerequisites & Dependencies](#prerequisites--dependencies)
- [Building & Running](#building--running)
- [Roadmap](#roadmap)
- [References & Acknowledgments](#references--acknowledgments)

---

## Overview

The **CHIP-8** is an interpreted programming language developed in the mid-1970s by Joseph Weisbecker for 8-bit microcomputers such as the COSMAC VIP and Telmac 1800. It provided a simple runtime environment for developing 2D games and graphics programs.

This project implements a complete, modern CHIP-8 interpreter that accurately recreates the virtual hardware specifications:
- Executes instructions through an authentic **Fetch-Decode-Execute** pipeline at ~600 Hz.
- Renders the classic 64×32 monochrome display buffer scaled up to **1280×640** with cyan-on-black retro aesthetics.
- Accurately tracks sprite collisions and XOR screen drawing.
- Synchronizes delay and sound timers to a strict **60 Hz** countdown.
- Handles both non-blocking key checks and blocking wait-for-key instructions.

---

## Key Features

- **Full CHIP-8 Instruction Set**: Complete support for all 34 core instructions (ALU math, bitwise operations, jumps, subroutines, BCD conversion, register dumps, and input skips).
- **Modern C++20 Design**: Leverages modern idioms including `std::array` for fixed memory buffers, `std::mt19937` for uniform pseudorandom generation, structured event handling, and clean RAII.
- **SFML 3.1.0 Rendering**: Leverages the latest SFML 3 API, automatically fetched and built via CMake `FetchContent` for a zero-manual-dependency developer setup.
- **Accurate Timing**: Configured to run at **60 FPS** with **10 CPU cycles per frame** (~600 Hz clock speed), matching the expected gameplay speed of classic CHIP-8 titles.
- **Deterministic Memory Model**: Full 4 KB address space with font sprites pre-loaded in interpreter memory (`0x000`–`0x04F`) and ROM programs loaded at `0x200`.
- **Automated Asset Management**: CMake automatically copies ROM directories to the build directory post-compilation for seamless execution.

---

## Virtual Machine Architecture

```
+-----------------------------------------------------------------------+
|                             CHIP-8 CPU                                |
|                                                                       |
|   +-----------------------+     +---------------------------------+   |
|   | General Registers     |     | Program Counter (PC): 16-bit    |   |
|   | V0, V1, ..., VE, VF   |     | Index Register  (I) : 16-bit    |   |
|   | (16 x 8-bit)          |     | Stack Pointer   (SP):  8-bit    |   |
|   +-----------------------+     +---------------------------------+   |
|                                                                       |
|   +-----------------------+     +---------------------------------+   |
|   | Call Stack            |     | Hardware Timers (60 Hz)         |   |
|   | (16 x 16-bit levels)  |     | Delay Timer (DT), Sound (ST)    |   |
|   +-----------------------+     +---------------------------------+   |
|                                                                       |
|   +-----------------------+     +---------------------------------+   |
|   | Pseudorandom (PRNG)   |     | ALU & Opcode Decoder            |   |
|   | std::mt19937 (C++20)  |     | 34 Standard Opcodes             |   |
|   +-----------------------+     +---------------------------------+   |
+-----------------------------------------------------------------------+
                                    |
          +-------------------------+-------------------------+
          |                                                   |
          v                                                   v
+-------------------+                               +-------------------+
|    4 KB Memory    |                               |  64x32 Framebuf   |
| 0x000-0x1FF: Font |                               | Monochrome Bitmap |
| 0x200-0xFFF: ROM  |                               | XOR Draw / SFML 3 |
+-------------------+                               +-------------------+
```

### Memory Layout

| Memory Range | Size | Purpose |
| :--- | :--- | :--- |
| `0x000` - `0x04F` | 80 Bytes | Built-in 4x5 hex fontset sprites (`0` through `F`) |
| `0x050` - `0x1FF` | 432 Bytes | Reserved interpreter memory |
| `0x200` - `0xFFF` | 3,584 Bytes | Program space (ROM instructions and application RAM) |

---

## Implemented Instruction Set

The emulator implements **all 34 standard CHIP-8 instructions**:

### Flow & Subroutines
| Opcode | Mnemonic | Description |
| :--- | :--- | :--- |
| `00E0` | `CLS` | Clears the display buffer. |
| `00EE` | `RET` | Returns from subroutine by restoring `PC` from stack and decrementing `SP`. |
| `1NNN` | `JP addr` | Unconditional jump to memory address `NNN`. |
| `2NNN` | `CALL addr` | Calls subroutine at `NNN`; pushes current `PC` to stack and increments `SP`. |
| `BNNN` | `JP V0, addr` | Jumps to address `NNN + V0`. |

### Conditional Branching
| Opcode | Mnemonic | Description |
| :--- | :--- | :--- |
| `3XKK` | `SE Vx, byte` | Skips next instruction if register `Vx == KK`. |
| `4XKK` | `SNE Vx, byte` | Skips next instruction if register `Vx != KK`. |
| `5XY0` | `SE Vx, Vy` | Skips next instruction if register `Vx == Vy`. |
| `9XY0` | `SNE Vx, Vy` | Skips next instruction if register `Vx != Vy`. |

### Register & ALU Operations
| Opcode | Mnemonic | Description |
| :--- | :--- | :--- |
| `6XKK` | `LD Vx, byte` | Loads constant `KK` into register `Vx`. |
| `7XKK` | `ADD Vx, byte` | Adds `KK` to `Vx` (carry flag `VF` is unchanged). |
| `8XY0` | `LD Vx, Vy` | Stores the value of `Vy` in `Vx`. |
| `8XY1` | `OR Vx, Vy` | Sets `Vx = Vx | Vy`. |
| `8XY2` | `AND Vx, Vy` | Sets `Vx = Vx & Vy`. |
| `8XY3` | `XOR Vx, Vy` | Sets `Vx = Vx ^ Vy`. |
| `8XY4` | `ADD Vx, Vy` | Adds `Vy` to `Vx`. Sets `VF = 1` on overflow (> 255), otherwise `0`. |
| `8XY5` | `SUB Vx, Vy` | Subtracts `Vy` from `Vx`. Sets `VF = 1` when NOT borrow (`Vx > Vy`), else `0`. |
| `8XY6` | `SHR Vx` | Shifts `Vx` right by 1 bit. Sets `VF` to the least significant bit prior to shift. |
| `8XY7` | `SUBN Vx, Vy` | Sets `Vx = Vy - Vx`. Sets `VF = 1` when NOT borrow (`Vy > Vx`), else `0`. |
| `8XYE` | `SHL Vx` | Shifts `Vx` left by 1 bit. Sets `VF` to the most significant bit prior to shift. |

### Memory & Index Register
| Opcode | Mnemonic | Description |
| :--- | :--- | :--- |
| `ANNN` | `LD I, addr` | Sets index register `I` to address `NNN`. |
| `FX1E` | `ADD I, Vx` | Adds `Vx` to index register `I` (`I = I + Vx`). |
| `FX29` | `LD F, Vx` | Sets `I` to the memory address of the 5-byte font character in `Vx`. |
| `FX33` | `LD B, Vx` | Stores Binary-Coded Decimal (BCD) of `Vx` in memory at `I`, `I+1`, and `I+2`. |
| `FX55` | `LD [I], Vx` | Stores registers `V0` through `Vx` into memory starting at address `I`. |
| `FX65` | `LD Vx, [I]` | Reads memory starting at address `I` into registers `V0` through `Vx`. |

### Graphics, Input & Timers
| Opcode | Mnemonic | Description |
| :--- | :--- | :--- |
| `CXKK` | `RND Vx, byte` | Sets `Vx` to a random byte masked with `KK` (`rand() & KK`). |
| `DXYN` | `DRW Vx, Vy, N` | Draws an `N`-byte sprite at coordinate `(Vx, Vy)`. Pixels are XORed. Sets `VF = 1` on collision. |
| `EX9E` | `SKP Vx` | Skips next instruction if key corresponding to value in `Vx` is pressed. |
| `EXA1` | `SKNP Vx` | Skips next instruction if key corresponding to value in `Vx` is not pressed. |
| `FX07` | `LD Vx, DT` | Sets `Vx` to current value of Delay Timer. |
| `FX0A` | `LD Vx, K` | Waits for a keypress (blocking execution) and stores pressed key value in `Vx`. |
| `FX15` | `LD DT, Vx` | Sets Delay Timer to `Vx`. |
| `FX18` | `LD ST, Vx` | Sets Sound Timer to `Vx`. |

---

## Keypad Mapping

The original CHIP-8 utilized a 16-key hexadecimal keypad (0x0 through 0xF). These keys are mapped to modern standard QWERTY keyboards:

```text
  Original CHIP-8 Keypad              Modern Keyboard (QWERTY)
+-----+-----+-----+-----+           +-----+-----+-----+-----+
|  1  |  2  |  3  |  C  |           |  1  |  2  |  3  |  4  |
+-----+-----+-----+-----+           +-----+-----+-----+-----+
|  4  |  5  |  6  |  D  |    --->   |  Q  |  W  |  E  |  R  |
+-----+-----+-----+-----+           +-----+-----+-----+-----+
|  7  |  8  |  9  |  E  |           |  A  |  S  |  D  |  F  |
+-----+-----+-----+-----+           +-----+-----+-----+-----+
|  A  |  0  |  B  |  F  |           |  Z  |  X  |  C  |  V  |
+-----+-----+-----+-----+           +-----+-----+-----+-----+
```

---

## Technical Implementation Details

### 1. Cycle & Timer Synchronization
- **CPU Cycle**: CHIP-8 does not define a fixed hardware frequency; historically, programs run smoothly between 500 Hz and 700 Hz. Here, 10 instructions are executed per frame inside a 60 FPS loop, yielding a rock-solid **600 Hz** CPU clock rate.
- **Hardware Timers**: CHIP-8 specifies both Delay and Sound timers count down at **60 Hz**. This is synchronized precisely to the SFML frame update loop via `updateDelayTimer()` and `updateSoundTimer()`.

### 2. XOR Sprite Rendering & Wrapping
- Sprites are drawn using XOR logic. If a pixel is flipped from white to black due to XORing, a collision is flagged by setting register `VF = 1`.
- Pixel coordinates wrap around the edges (`% 64` horizontally and `% 32` vertically), preventing out-of-bounds memory corruption while keeping compatibility with games that rely on edge-wrapping.

### 3. Modern C++ Standard Practices
- **Fixed-size Buffers**: Using `std::array<uint8_t, 4096>` and `std::array<uint8_t, 2048>` to eliminate heap allocations during emulation loops.
- **High-Quality PRNG**: Employs `std::mt19937` seeded with `std::random_device` and a uniform byte distribution `std::uniform_int_distribution<uint8_t>(0, 255)` for unbiased random numbers.
- **SFML 3 Modern Event API**: Uses type-safe `event->is<sf::Event::Closed>()` matching introduced in SFML 3.

---

## Project Structure

```text
.
├── CMakeLists.txt              # Build configuration with SFML 3.1 FetchContent
├── include/
│   └── chip8.h                 # Chip8 class definition, keymaps, and fontset
├── src/
│   ├── main.cpp                # SFML window loop, timing, and rendering
│   ├── chip8.cpp               # Core cycle, fetch/decode routing, ROM loader
│   └── chip8-instructions.cpp # Complete implementation of all 34 opcodes
└── roms/                       # Directory for CHIP-8 ROM binaries (.ch8)
```

---

## Prerequisites & Dependencies

### Tools Required
- **C++20 compliant compiler** (GCC 11+, Clang 13+, or MSVC 2019+)
- **CMake 3.28+**
- **Git** (for fetching SFML via CMake `FetchContent`)

### Linux Dependencies (SFML 3)
On Debian/Ubuntu-based distributions, install the graphics and windowing development libraries required to build SFML from source:

```bash
sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libgl1-mesa-dev \
    libfreetype-dev \
    libflac-dev \
    libogg-dev \
    libvorbis-dev
```

---

## Building & Running

### 1. Clone the Repository

```bash
git clone https://github.com/ArchanjoGabriel/chip8-emulator.git
cd chip8-emulator
```

### 2. Add ROMs
Place any standard `.ch8` binary ROM files into the `roms/` folder:

```bash
mkdir -p roms
cp /path/to/your/game.ch8 roms/
```

### 3. Configure and Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target chip8_emulator --config Release
```

> **Note**: On the first build, CMake will automatically download and compile SFML 3.1.0 using `FetchContent`. The `roms/` directory is automatically copied into the build output directory post-build.

### 4. Run a ROM

```bash
./build/chip8_emulator roms/game.ch8
```

*(On Windows, run `.\build\Release\chip8_emulator.exe roms\game.ch8`)*

---

## Roadmap

- [x] Complete 34-instruction CHIP-8 opcode set
- [x] SFML 3 graphical display scaling (20x, 1280x640)
- [x] Accurate 60 Hz delay & sound timer decrementing
- [x] 16-key keypad input handling & blocking keywait
- [ ] Audio tone buzzer implementation via `sf::Sound` / `sf::SoundBuffer` when sound timer > 0

---

## References & Acknowledgments

- [Cowgod's CHIP-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) by Thomas P. Greene
- [Timendus CHIP-8 Test Suite](https://github.com/Timendus/chip8-test-suite) for validation and accuracy testing
- [SFML (Simple and Fast Multimedia Library)](https://www.sfml-dev.org/)
