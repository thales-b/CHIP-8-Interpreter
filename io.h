#ifndef IO_H
#define IO_H

#include "chip8.h"

void keypress(Chip8 *chip8);
uint8_t get_key();
void sound(Chip8 *chip);

#endif