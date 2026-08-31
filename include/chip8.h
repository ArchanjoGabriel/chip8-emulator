#ifndef CHIP8_EMULATOR_CHIP8_H
#define CHIP8_EMULATOR_CHIP8_H
#include <array>
#include <cstdint>
#include <random>
#include <string>

#include "SFML/Window/Keyboard.hpp"

constexpr std::array chip8_keymap = {
    sf::Keyboard::Key::X,    // 0
    sf::Keyboard::Key::Num1, // 1
    sf::Keyboard::Key::Num2, // 2
    sf::Keyboard::Key::Num3, // 3
    sf::Keyboard::Key::Q,    // 4
    sf::Keyboard::Key::W,    // 5
    sf::Keyboard::Key::E,    // 6
    sf::Keyboard::Key::A,    // 7
    sf::Keyboard::Key::S,    // 8
    sf::Keyboard::Key::D,    // 9
    sf::Keyboard::Key::Z,    // A
    sf::Keyboard::Key::C,    // B
    sf::Keyboard::Key::Num4, // C
    sf::Keyboard::Key::R,    // D
    sf::Keyboard::Key::F,    // E
    sf::Keyboard::Key::V     // F
};

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

    // Random number generator (RNG)
    std::mt19937 rng;
    std::uniform_int_distribution<uint8_t> dist;
    uint8_t genRandomNumber();

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
    void OP_00EE();
    void OP_1NNN();
    void OP_2NNN();
    void OP_3XKK();
    void OP_4XKK();
    void OP_5XY0();
    void OP_6XKK();
    void OP_7XKK();
    void OP_8XY0();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();
    void OP_9XY0();
    void OP_ANNN();
    void OP_BNNN();
    void OP_CXKK();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();
    void OP_FX07();
    void OP_FX15();
    void OP_FX18();

public:

    Chip8();
    void cycle();
    void loadROM(const std::string& path);
    std::array<uint8_t, 64 * 32> getDisplay();
    void updateDelayTimer();
    void updateSoundTimer();
};

#endif //CHIP8_EMULATOR_CHIP8_H
