#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H
#include <array>
#include <cstdint>

class Chip8 {
    // Memory
    std::array<uint8_t, 4096> memory;

    // Registers
    std::array<uint8_t, 16> V;
    uint16_t I;
    uint16_t PC;

    // Timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // Stack
    uint8_t SP;
    std::array<uint16_t, 16> stack;

    // Opcode
    uint16_t opcode;

    // Display
    std::array<uint8_t, 64 * 32> display;

    // Fetch cycle
    void fetch();

    // Decode cycle
    uint16_t getNNN();
    uint8_t getN();
    uint8_t getX();
    uint8_t getY();
    uint8_t getKK();
    uint8_t getFirstNibble();

    // Execute cycle
    void execute();

    // Instructions
    void OP_00E0();
    void OP_1NNN();
    void OP_6XKK();
    void OP_7XKK();
    void OP_ANNN();

public:

    Chip8();
};

#endif //CHIP8_EMULATOR_CHIP8_H
