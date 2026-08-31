#include "chip8.h"

#include <stdexcept>
#include <fstream>

constexpr uint8_t CHIP8_FONTSET[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
    : I(0),
      PC(0x200),
      delay_timer(0),
      sound_timer(0),
      SP(0),
      opcode(0)
{
    memory.fill(0);
    V.fill(0);
    display.fill(0);
    stack.fill(0);

    for (int i = 0; i < 80; i++) {
        memory[0x200+i] = CHIP8_FONTSET[i];
    }
}

void Chip8::fetch() {
    opcode = (memory[PC] << 8) | memory[PC+1];
    PC += 2;
}

uint16_t Chip8::getNNN() {
    return (opcode & 0x0FFF);
}

uint8_t Chip8::getN() {
    return (opcode & 0x000F);
}

uint8_t Chip8::getX() {
    return (opcode & 0x0F00) >> 8;
}

uint8_t Chip8::getY() {
    return (opcode & 0x00F0) >> 4;
}

uint8_t Chip8::getKK() {
    return (opcode & 0x00FF);
}

uint8_t Chip8::getFirstNibble() {
    return (opcode & 0xF000) >> 12;
}

void Chip8::execute() {
    switch (getFirstNibble()) {
        case 0x0:
            if (opcode == 0x00E0) OP_00E0();
            else if (opcode == 0x00EE) OP_00EE();
            break;
        case 0x1: OP_1NNN();
            break;
        case 0x2: OP_2NNN();
            break;
        case 0x3: OP_3XKK();
            break;
        case 0x4: OP_4XKK();
            break;
        case 0x5: OP_5XY0();
            break;
        case 0x6: OP_6XKK();
            break;
        case 0x7: OP_7XKK();
            break;
        case 0x8:
            if (getN() == 0x0) OP_8XY0();
            else if (getN() == 0x1) OP_8XY1();
            else if (getN() == 0x2) OP_8XY2();
            else if (getN() == 0x3) OP_8XY3();
            break;
        case 0x9: OP_9XY0();
            break;
        case 0xA: OP_ANNN();
            break;
        case 0xD: OP_DXYN();
    }
}

void Chip8::cycle() {
    fetch();
    execute();
}

void Chip8::loadROM(const std::string &path) {
    constexpr uint16_t PROGRAM_START = 0x200;

    std::ifstream file(path, std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Could not open the ROM: " + path);
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size > memory.size() - PROGRAM_START)
    {
        throw std::runtime_error("ROM too large for CHIP-8 memory");
    }

    file.read(
        reinterpret_cast<char*>(memory.data() + PROGRAM_START),
        size
    );

    if (!file)
    {
        throw std::runtime_error("Error reading the ROM: " + path);
    }
}

std::array<uint8_t, 64 * 32> Chip8::getDisplay() {
    return display;
}
