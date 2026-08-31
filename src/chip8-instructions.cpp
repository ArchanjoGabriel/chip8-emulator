#include "chip8.h"

void Chip8::OP_00E0() {
    display.fill(0);
}

void Chip8::OP_00EE() {
    PC = stack[SP];
    SP--;
}

void Chip8::OP_1NNN() {
    PC = getNNN();
}

void Chip8::OP_2NNN() {
    SP++;
    stack[SP] = PC;
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

void Chip8::OP_DXYN() {
    V[0xF] = 0;

    uint8_t x = getX();
    uint8_t y = getY();
    uint8_t n = getN();

    for (uint8_t row = 0; row < n; row++) {
        uint8_t sprite = memory[I + row];

        for (uint8_t col = 0; col < 8; col++) {
            if (sprite & (0x80 >> col)) {
                uint8_t px = (V[x] + col) % 64;
                uint8_t py = (V[y] + row) % 32;

                uint16_t index = py * 64 + px;

                if (display[index]) V[0xF] = 1;

                display[index] ^= 1;
            }
        }
    }
}
