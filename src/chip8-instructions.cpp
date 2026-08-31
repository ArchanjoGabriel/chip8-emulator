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

void Chip8::OP_3XKK() {
    uint8_t x = getX();
    uint8_t kk = getKK();

    if (V[x] == kk) PC += 2;
}

void Chip8::OP_4XKK() {
    uint8_t x = getX();
    uint8_t kk = getKK();

    if (V[x] != kk) PC += 2;
}

void Chip8::OP_5XY0() {
    uint8_t x = getX();
    uint8_t y = getY();

    if (V[x] == V[y]) PC += 2;
}

void Chip8::OP_6XKK() {
    uint8_t x = getX();
    V[x] = getKK();
}

void Chip8::OP_7XKK() {
    uint8_t x = getX();
    V[x] += getKK();
}

void Chip8::OP_8XY0() {
    uint8_t x = getX();
    uint8_t y = getY();

    V[x] = V[y];
}

void Chip8::OP_8XY1() {
    uint8_t x = getX();
    uint8_t y = getY();

    V[x] |= V[y];
}

void Chip8::OP_8XY2() {
    uint8_t x = getX();
    uint8_t y = getY();

    V[x] &= V[y];
}

void Chip8::OP_8XY3() {
    uint8_t x = getX();
    uint8_t y = getY();

    V[x] ^= V[y];
}

void Chip8::OP_8XY4() {
    uint8_t x = getX();
    uint8_t y = getY();

    uint16_t sum = V[x] + V[y];
    V[0xF] = (sum > 0xFF) ? 1 : 0;
    V[x] = sum & 0xFF;
}

void Chip8::OP_8XY5() {
    uint8_t x = getX();
    uint8_t y = getY();

    V[0xF] = (V[x] > V[y]) ? 1 : 0;
    V[x] -= V[y];
}

void Chip8::OP_8XY6() {
    uint8_t x = getX();

    V[0xF] = (V[x] & 0x1) ? 1 : 0;
    V[x] >>= 1;
}

void Chip8::OP_8XYE() {
    uint8_t x = getX();

    V[0xF] = ((V[x] & 0xF0) >> 7) ? 1 : 0;
    V[x] <<= 1;
}

void Chip8::OP_8XY7() {
    uint8_t x = getX();
    uint8_t y = getY();

    V[0xF] = (V[y] > V[x]) ? 1 : 0;
    V[x] = V[y] - V[x];
}

void Chip8::OP_9XY0() {
    uint8_t x = getX();
    uint8_t y = getY();

    if (V[x] != V[y]) PC += 2;
}

void Chip8::OP_ANNN() {
    I = getNNN();
}

void Chip8::OP_BNNN() {
    PC = getNNN() + V[0x0];
}

void Chip8::OP_CXKK() {
    uint8_t x = getX();
    uint8_t kk = getKK();
    uint8_t rd = genRandomNumber();

    V[x] = rd & kk;
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

void Chip8::OP_EX9E() {
    uint8_t x = getX();
    uint8_t key_value = V[x];

    if (key_value < 16) {
        sf::Keyboard::Key sfml_key = chip8_keymap[key_value];
        if (sf::Keyboard::isKeyPressed(sfml_key)) PC += 2;
    }
}

void Chip8::OP_EXA1() {
    uint8_t x = getX();
    uint8_t key_value = V[x];

    if (key_value < 16) {
        sf::Keyboard::Key sfml_key = chip8_keymap[key_value];
        if (!sf::Keyboard::isKeyPressed(sfml_key)) PC += 2;
    }
}

void Chip8::OP_FX07() {
    uint8_t x = getX();

    V[x] = delay_timer;
}

void Chip8::OP_FX15() {
    uint8_t x = getX();

    delay_timer = V[x];
}

void Chip8::OP_FX18() {
    uint8_t x = getX();

    sound_timer = V[x];
}

void Chip8::OP_FX1E() {
    uint8_t x = getX();

    I += V[x];
}

void Chip8::OP_FX29() {
    uint8_t x = getX();

    I = V[x] * 5;
}

void Chip8::OP_FX33() {
    uint8_t x = getX();
    uint8_t value = V[x];

    memory[I]     = value / 100;
    memory[I + 1] = (value/10) % 10;
    memory[I + 2] = value % 10;
}
