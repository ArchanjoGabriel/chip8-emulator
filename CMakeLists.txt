cmake_minimum_required(VERSION 3.28)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(chip8_emulator
        src/main.cpp
        src/chip8.cpp
)

target_include_directories(chip8_emulator
        PRIVATE
            include
)
