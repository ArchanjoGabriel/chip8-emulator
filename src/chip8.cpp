#include "chip8.h"

Chip8::Chip8()
    : I(0),
      PC(0),
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
