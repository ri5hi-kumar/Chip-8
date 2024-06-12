#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "instructions.h"

void load_rom(Chip8 *chip8, const char *rom_file);
void initialize_chip8(Chip8 *chip8);
void emulate_cycle(Chip8 *chip8);
void handle_input(Chip8 * chip8);

#endif
