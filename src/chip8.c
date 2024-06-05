#include "chip8.h"

void load_rom(Chip8 *chip8, const char *rom_file) {
    long rom_length;
    unsigned char *rom_buffer;

    FILE *rom = fopen(rom_file, "rb");

    if(rom != NULL) {
        // find size of the file
        fseek(rom, 0, SEEK_END);
        rom_length = ftell(rom);
        rewind(rom);

        // allocate memory for the rom file
        rom_buffer = (unsigned char *)malloc(sizeof(unsigned char) * rom_length);
        if(rom_buffer == NULL) {
            printf("Memory not allocated\n");
            exit(EXIT_FAILURE);
        }

        // read rom into the buffer
        fread(rom_buffer, sizeof(unsigned char), rom_length, rom);

        // copy rom file into the chip8 memory
        if((PROGRAM_END_ADDR - PROGRAM_START_ADDR) >= rom_length) {
            for(int i = 0; i < rom_length; i++) {
                chip8->memory[i + 512] = rom_buffer[i];     // chip8 program memory starts from 0x200
            }
        } else {
            printf("ROM size too large\n");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("ROM file doesn't exist\n");
        exit(EXIT_FAILURE);
    }
}


void initialize_chip8(Chip8 *chip8) {
    chip8->PC = PC_START;
    chip8->SP = 0;
    chip8->opcode = 0;
    chip8->I = 0;
    chip8->sound_timer = 0;
    chip8->delay_timer = 0;

    // clearing the display
    for(int i = 0; i < 32; i++) {
        for(int j = 0; j < 64; j++) {
            chip8->gfx[i][j] = 0;
        }
    }

    // clear memory
    for(int i = 0; i < 4096; i++) {
        chip8->memory[i] = 0;
    }

    // clear stack
    for(int i = 0; i < 16; i++) {
        chip8->stack[i] = 0;
    }
}

void emulate_cycle(Chip8 *chip8) {
    // fetch opcode from the rom memory which is at PC and PC + 1 (opcode is of 3 bytes)
    chip8->opcode = chip8->memory[chip8->PC] << 8 | chip8->memory[chip8-> PC + 1];

    // decode the opcode
    // CHIP-8’s index register and program counter can only address 12 bits (conveniently), which is 4096 addresses.
    switch(chip8->opcode & 0xF000) {
        case 0x0000:
    }
}