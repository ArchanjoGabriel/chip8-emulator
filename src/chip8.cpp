#include "chip8.h"

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
