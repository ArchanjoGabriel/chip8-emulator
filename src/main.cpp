#include <iostream>

#include "chip8.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

constexpr int WIDTH = 64;
constexpr int HEIGHT = 32;
constexpr int SCALE = 20;

int main(int argc, char *argv[]) {
    if (argc != 2)
        throw std::runtime_error("Invalid number of arguments");

    std::string romLocation = argv[1];

    std::cout << "CHIP-8 emulator!" << std::endl;

    sf::RenderWindow window(sf::VideoMode({WIDTH*SCALE, HEIGHT*SCALE}), "CHIP-8 Emulator");
    window.setFramerateLimit(60);

    sf::RectangleShape pixel;
    pixel.setSize({SCALE, SCALE});
    pixel.setFillColor(sf::Color::Cyan);

    Chip8 chip8;
    chip8.loadROM(romLocation);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        chip8.cycle();
        auto display = chip8.getDisplay();
        chip8.updateDelayTimer();
        chip8.updateSoundTimer();

        window.clear(sf::Color::Black);

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (display[y * WIDTH + x]) {
                    pixel.setPosition({static_cast<float>(x * SCALE), static_cast<float>(y * SCALE)});
                    window.draw(pixel);
                }
            }
        }

        window.display();
    }

    return 0;
}
