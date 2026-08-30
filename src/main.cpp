#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"

constexpr int WIDTH = 64;
constexpr int HEIGHT = 32;
constexpr int SCALE = 20;

int main() {
    std::cout << "CHIP-8 emulator!" << std::endl;

    sf::RenderWindow window(sf::VideoMode({WIDTH*SCALE, HEIGHT*SCALE}), "CHIP-8 Emulator");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);


        window.display();
    }

    return 0;
}
