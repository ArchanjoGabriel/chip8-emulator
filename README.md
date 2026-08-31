# CHIP-8 Emulator

A CHIP-8 emulator written in modern C++20, built from scratch to explore emulation, computer architecture and low-level programming.

The project currently implements the core emulator structure, ROM loading, a fetch/decode/execute cycle, timer updates, keyboard handling, and SFML-based rendering for the classic CHIP-8 64x32 monochrome display.

## Current Features

- 4 KB CHIP-8 memory layout
- 16 general-purpose 8-bit registers (`V0` to `VF`)
- Index register (`I`) and program counter (`PC`)
- Stack storage and stack pointer for subroutine calls
- Delay and sound timer updates at the render frame rate
- Binary ROM loading at address `0x200`
- Built-in CHIP-8 fontset loaded into interpreter memory
- 64x32 display buffer
- SFML window rendering with scaled pixels
- Sprite drawing with XOR behavior and collision flag support
- 16-key CHIP-8 keypad mapped to a modern keyboard layout
- Random byte generation for `CXKK`
- 10 CPU cycles executed per rendered frame

## Implemented Opcodes

The emulator currently supports the main CHIP-8 instruction groups needed by many simple ROMs:

| Opcode | Instruction | Description |
| --- | --- | --- |
| `00E0` | `CLS` | Clear the display |
| `00EE` | `RET` | Return from a subroutine |
| `1NNN` | `JP addr` | Jump to address `NNN` |
| `2NNN` | `CALL addr` | Call subroutine at `NNN` |
| `3XKK` | `SE Vx, byte` | Skip next instruction if `Vx == KK` |
| `4XKK` | `SNE Vx, byte` | Skip next instruction if `Vx != KK` |
| `5XY0` | `SE Vx, Vy` | Skip next instruction if `Vx == Vy` |
| `6XKK` | `LD Vx, byte` | Set register `Vx` to `KK` |
| `7XKK` | `ADD Vx, byte` | Add `KK` to register `Vx` |
| `8XY0` | `LD Vx, Vy` | Set `Vx` to `Vy` |
| `8XY1` | `OR Vx, Vy` | Set `Vx` to `Vx OR Vy` |
| `8XY2` | `AND Vx, Vy` | Set `Vx` to `Vx AND Vy` |
| `8XY3` | `XOR Vx, Vy` | Set `Vx` to `Vx XOR Vy` |
| `8XY4` | `ADD Vx, Vy` | Add `Vy` to `Vx` and set carry flag |
| `8XY5` | `SUB Vx, Vy` | Subtract `Vy` from `Vx` and set borrow flag |
| `8XY6` | `SHR Vx` | Shift `Vx` right by one bit |
| `8XY7` | `SUBN Vx, Vy` | Set `Vx` to `Vy - Vx` and set borrow flag |
| `8XYE` | `SHL Vx` | Shift `Vx` left by one bit |
| `9XY0` | `SNE Vx, Vy` | Skip next instruction if `Vx != Vy` |
| `ANNN` | `LD I, addr` | Set index register `I` to `NNN` |
| `BNNN` | `JP V0, addr` | Jump to `NNN + V0` |
| `CXKK` | `RND Vx, byte` | Set `Vx` to a random byte masked by `KK` |
| `DXYN` | `DRW Vx, Vy, nibble` | Draw an `N`-byte sprite at `(Vx, Vy)` |
| `EX9E` | `SKP Vx` | Skip next instruction if the key in `Vx` is pressed |
| `EXA1` | `SKNP Vx` | Skip next instruction if the key in `Vx` is not pressed |
| `FX07` | `LD Vx, DT` | Set `Vx` to the delay timer value |
| `FX0A` | `LD Vx, K` | Wait for a key press and store it in `Vx` |
| `FX15` | `LD DT, Vx` | Set the delay timer to `Vx` |
| `FX18` | `LD ST, Vx` | Set the sound timer to `Vx` |
| `FX1E` | `ADD I, Vx` | Add `Vx` to `I` |
| `FX29` | `LD F, Vx` | Set `I` to the font sprite address for digit `Vx` |
| `FX33` | `LD B, Vx` | Store the BCD representation of `Vx` in memory at `I` |
| `FX55` | `LD [I], Vx` | Store registers `V0` through `Vx` in memory starting at `I` |
| `FX65` | `LD Vx, [I]` | Load registers `V0` through `Vx` from memory starting at `I` |

## Keyboard Layout

The CHIP-8 keypad is mapped to the keyboard as follows:

```text
CHIP-8 keypad        Keyboard
1 2 3 C              1 2 3 4
4 5 6 D              Q W E R
7 8 9 E              A S D F
A 0 B F              Z X C V
```

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

The application opens an SFML window with a 64x32 CHIP-8 display scaled by `20x`. The emulator executes 10 CPU cycles per frame and updates the delay and sound timers once per frame.

## Current Limitations

- The full CHIP-8 instruction set is not implemented yet.
- Sound output is not implemented yet.
- Timer countdown currently follows the SFML render frame rate.
- ROM compatibility is still limited to programs that only use the currently implemented opcodes.

## Roadmap

- Complete the CHIP-8 opcode set
- Add audio feedback for the sound timer
- Improve error handling and diagnostics for unsupported opcodes
- Add automated tests for instruction behavior
