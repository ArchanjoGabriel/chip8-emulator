# CHIP-8 Emulator

A small CHIP-8 emulator written in modern C++ as a portfolio project.

The project currently implements the core emulator structure, ROM loading, a basic fetch/decode/execute cycle, and SFML-based rendering for the classic CHIP-8 64x32 monochrome display.

## Current Features

- 4 KB CHIP-8 memory layout
- 16 general-purpose 8-bit registers (`V0` to `VF`)
- Index register (`I`) and program counter (`PC`)
- Stack storage for future subroutine support
- Delay and sound timer fields
- Binary ROM loading at address `0x200`
- 64x32 display buffer
- SFML window rendering with scaled pixels
- Sprite drawing with XOR behavior and collision flag support

## Implemented Opcodes

The emulator is still in an early stage and currently supports a focused subset of CHIP-8 instructions:

| Opcode | Instruction | Description |
| --- | --- | --- |
| `00E0` | `CLS` | Clear the display |
| `1NNN` | `JP addr` | Jump to address `NNN` |
| `6XKK` | `LD Vx, byte` | Set register `Vx` to `KK` |
| `7XKK` | `ADD Vx, byte` | Add `KK` to register `Vx` |
| `ANNN` | `LD I, addr` | Set index register `I` to `NNN` |
| `DXYN` | `DRW Vx, Vy, nibble` | Draw an `N`-byte sprite at `(Vx, Vy)` |

## Tech Stack

- C++20
- CMake 3.28+
- SFML 3.1.0, fetched automatically through CMake `FetchContent`

## Project Structure

```text
.
├── CMakeLists.txt
├── include/
│   └── chip8.h
├── src/
│   ├── main.cpp
│   ├── chip8.cpp
│   └── chip8-instructions.cpp
└── roms/
```

## ROMs

Create a `roms` directory in the project root and place your CHIP-8 ROM files there:

```bash
mkdir -p roms
cp /path/to/ROM_NAME.ch8 roms/
```

During the build, CMake copies the `roms` directory from the project root into the executable output directory. This lets the emulator load ROMs using the same relative path from inside the build directory.

## Building

From the project root:

```bash
cmake -S . -B build
cmake --build build --target chip8_emulator
```

CMake downloads and builds SFML automatically during configuration.

## Running

After building, enter the build directory and run the emulator with a ROM from the copied `roms` folder:

```bash
cd build
./chip8_emulator roms/ROM_NAME.ch8
```

The application opens an SFML window with a 64x32 CHIP-8 display scaled by `20x`.

## Current Limitations

- The full CHIP-8 instruction set is not implemented yet.
- Keyboard input is not implemented yet.
- Timer countdown behavior is not implemented yet.
- Sound output is not implemented yet.
- ROM compatibility is limited to programs that only use the currently implemented opcodes.

## Roadmap

- Complete the CHIP-8 opcode set
- Add keypad input mapping
- Implement delay and sound timer updates
- Add audio feedback for the sound timer
- Improve error handling and diagnostics for unsupported opcodes
- Add automated tests for instruction behavior

