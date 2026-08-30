#include "chip8.h"

#include <stdexcept>
#include <fstream>

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
            break;
        case 0x1: OP_1NNN();
            break;
        case 0x6: OP_6XKK();
            break;
        case 0x7: OP_7XKK();
            break;
        case 0xA: OP_ANNN();
            break;
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
