#ifndef CHIP8_CONTEXT_H
#define CHIP8_CONTEXT_H

#define PC_START 0x200
#define CHIP8_RAM_START_ADDR 0x000
#define CHIP8_RAM_END_ADDR 0x1FF
#define PROGRAM_START_ADDR 0x200
#define PROGRAM_END_ADDR 0xFFF

typedef struct Chip8
{
    unsigned short opcode;          // store the current opcode 2 bytes long
    unsigned char memory[4096];     // 4KB of memory
    unsigned char V[16];            // 16 general purpose registers
    unsigned short I;               // stores memory addresses, only lowest 12 bits used
    unsigned short PC;              // Program Counter register store the currently executing address
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned char SP;               // used to point to the topmost level of the stack
    unsigned short stack[16];       // used to store the address that the interpreter shoud return to when finished with a subroutine
    unsigned char gfx[64][32];      // display
    unsigned char key[16];          // keypad
} Chip8;

#endif