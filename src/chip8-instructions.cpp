#include "chip8.h"

void Chip8::OP_00E0() {
    display.fill(0);
}

void Chip8::OP_1NNN() {
    PC = getNNN();
}

void Chip8::OP_6XKK() {
    uint8_t x = getX();
    V[x] = getKK();
}

void Chip8::OP_7XKK() {
    uint8_t x = getX();
    V[x] += getKK();
}

void Chip8::OP_ANNN() {
    I = getNNN();
}
