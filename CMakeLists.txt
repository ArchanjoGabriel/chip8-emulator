cmake_minimum_required(VERSION 3.28)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM
)

FetchContent_MakeAvailable(SFML)

add_executable(chip8_emulator
        src/main.cpp
        src/chip8.cpp
        src/chip8-instructions.cpp
)

target_link_libraries(chip8_emulator PRIVATE SFML::Graphics)

target_include_directories(chip8_emulator
        PRIVATE
            include
)
