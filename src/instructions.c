#include "instructions.h"

/*
    nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
    n or nibble - A 4-bit value, the lowest 4 bits of the instruction
    x - A 4-bit value, the lower 4 bits of the high byte of the instruction
    y - A 4-bit value, the upper 4 bits of the low byte of the instruction
    kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/

/*
 * Clear the display.
 */
void cls(Chip8 *chip8) {
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 32; j++) {
            chip8->gfx[i][j] = 0;
        }
    }
    chip8->PC += 2;
}

/*
 * Return from a subroutine.
 * The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
 */
void ret(Chip8 *chip8) {
    chip8->PC = chip8->stack[chip8->SP];
    chip8->SP--;
    chip8->PC += 2;
}

/*
 * Jump to location nnn.
 * The interpreter sets the program counter to nnn.
 */
void jmp(Chip8 *chip8) {
    unsigned short nnn = chip8->opcode & 0x0FFF;
    chip8->PC = nnn;
}

/*
 * Call subroutine at nnn.
 * The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
 */
void call(Chip8 *chip8) {
    unsigned short nnn = chip8->opcode & 0x0FFF;
    chip8->SP++;
    chip8->stack[chip8->SP] = chip8->PC;
    chip8->PC = nnn;
}

/*
 * Skip next instruction if Vx = kk.
 * The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
 */
void se_Vx_kk(Chip8 *chip8) {
    unsigned char kk = chip8->opcode & 0x00FF;
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    if(chip8->V[x] == kk) {
        chip8->PC += 4;
    } else {
        chip8->PC += 2;
    }
}

/*
 * Skip next instruction if Vx != kk.
 * The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
 */
void sne_Vx_kk(Chip8 *chip8) {
    unsigned char kk = chip8->opcode & 0x00FF;
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    if(chip8->V[x] != kk) {
        chip8->PC += 4;
    } else {
        chip8->PC += 2;
    }
}

/*
 * Skip next instruction if Vx = Vy.
 * The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
 */
void se_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    if(chip8->V[x] == chip8->V[y]) {
        chip8->PC += 4;
    } else {
        chip8->PC += 2;
    }
}

/*
 * Set Vx = kk.
 * The interpreter puts the value kk into register Vx.
 */
void ld_Vx(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char kk = chip8->opcode & 0x00FF;
    chip8->V[x] = kk;
    chip8->PC += 2;
}

/*
 * Set Vx = Vx + kk.
 * Adds the value kk to the value of register Vx, then stores the result in Vx.
 */
void add_Vx_kk(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char kk = chip8->opcode & 0x00FF;
    chip8->V[x] += kk;
    chip8->PC += 2;
}

/*
 * Set Vx = Vy.
 * Stores the value of register Vy in register Vx.
 */
void ld_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    chip8->V[x] = chip8->V[y];
    chip8->PC += 2;
}

/*
 * Set Vx = Vx OR Vy.
 * Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
 * A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. 
 * Otherwise, it is 0.
 */
void or_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    chip8->V[x] |= chip8->V[y];
    chip8->PC += 2;
}

/*
 * Set Vx = Vx AND Vy.
 * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
 * A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1.
 * Otherwise, it is 0.
 */
void and_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    chip8->V[x] &= chip8->V[y];
    chip8->PC += 2;
}

/*
 * Set Vx = Vx XOR Vy.
 * Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
 * An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same,
 * then the corresponding bit in the result is set to 1. Otherwise, it is 0.
 */
void xor_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    chip8->V[x] ^= chip8->V[y];
    chip8->PC += 2;
}

/*
 * Set Vx = Vx + Vy, set VF = carry.
 * The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,)
 * VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
 */
void add_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    unsigned short sum = chip8->V[x] + chip8->V[y];
    chip8->V[x] = sum & 0xFF;
    if(sum > 255) {
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }
    chip8->PC += 2;
}

/*
 * Set Vx = Vx - Vy, set VF = NOT borrow.
 * If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
 */
void sub_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    if(chip8->V[x] > chip8->V[y]) {
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }
    chip8->V[x] -= chip8->V[y];
    chip8->PC += 2;
}

/*
 * Set Vx = Vx SHR 1.
 * If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
 */
void shr(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    chip8->V[0xF] = chip8->V[x] % 2 == 1 ? 1 : 0;
    chip8->V[x] >>= 1;
    chip8->PC += 2;
}

/*
 * Set Vx = Vy - Vx, set VF = NOT borrow.
 * If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
 */
void subn_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    if(chip8->V[x] < chip8->V[y]) {
        chip8->V[0xF] = 1;
    } else {
        chip8->V[0xF] = 0;
    }
    chip8->V[x] = chip8->V[y] - chip8->V[x];
    chip8->PC += 2;
}

/*
 * Set Vx = Vx SHL 1.
 * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
 */
void shl(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    chip8->V[0xF] = chip8->V[x] >> 7;
	chip8->V[x] <<= 1;
    chip8->PC += 2;
}

/*
 * Skip next instruction if Vx != Vy.
 * The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
 */
void sne_Vx_Vy(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char y = (chip8->opcode & 0x00F0) >> 4;
    if(chip8->V[x] != chip8->V[y]) {
        chip8->PC += 4;
    } else {
        chip8->PC += 2;
    }
}

/*
 * Set I = nnn.
 * The value of register I is set to nnn.
 */
void ldi(Chip8 *chip8) {
    unsigned short nnn = chip8->opcode & 0x0FFF;
    chip8->I = nnn;
    chip8->PC += 2;
}

/*
 * Jump to location nnn + V0.
 * The program counter is set to nnn plus the value of V0.
 */
void jmp_V0(Chip8 *chip8) {
    unsigned short nnn = chip8->opcode & 0x0FFF;
    chip8->PC = nnn + chip8->V[0x0];
}

/*
 * Set Vx = random byte AND kk.
 * The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk.
 * The results are stored in Vx. See instruction 8xy2 for more information on AND.
 */
void rnd(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    unsigned char kk = chip8->opcode & 0x00FF;
    srand(time(NULL));                        // Initialization, should only be called once.
    unsigned char r = rand() % 256;           // Returns a pseudo-random integer between 0 and RAND_MAX.
    chip8->V[x] = r & kk;
    chip8->PC += 2;
}

/*
 * Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
 * The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as
 * sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to
 * be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the
 * coordinates of the display, it wraps around to the opposite side of the screen.
 */
void drw(Chip8 *chip8) {
    unsigned char regX = (chip8->opcode & 0x0F00) >> 8;
    unsigned char regY = (chip8->opcode & 0x00F0) >> 4;
    int n = (chip8->opcode & 0x000F);
    int width = 8;

    chip8->V[0xF] = 0;

    for (int row = 0; row < n; row++) {
        // Get a row one of sprite data from the memory address in reg I (one byte per row)
        unsigned char spriteData = (chip8->memory[chip8->I + row]);

        // for each 8 pixels/bits in this sprite row
        for (int col = 0; col < width; col++) {
            // if the current pixel in the sprite row is on,
            // and the screen pixel at X,Y is on, set pixel to 0, set VF to 1
            if ((spriteData & 0x80) > 0) {
                int x = chip8->V[regX] + col;
                int y = chip8->V[regY] + row;
                if ((x >= 0 && x < 64) && (y >= 0 && y < 32)) {
                    if (chip8->gfx[x][y] == 1) {
                        chip8->V[0xF] = 1;
                    }
                    chip8->gfx[x][y] ^= 1;
                }
            }
            // Point spriteData to the next bit
            spriteData <<= 1;
        }
    }
    chip8->PC += 2;
}

/*
 * Skip next instruction if key with the value of Vx is pressed.
 * Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
 */
void skp(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    if(chip8->key[chip8->V[x]] != 0) {
        chip8->PC += 4;
    } else {
        chip8->PC += 2;
    }
}

/*
 * Skip next instruction if key with the value of Vx is not pressed.
 * Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
 */
void sknp(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    if(chip8->key[chip8->V[x]] == 0) {
        chip8->PC += 4;
    } else {
        chip8->PC += 2;
    }
}

/*
 * Set Vx = delay timer value.
 * The value of DT is placed into Vx.
 */
void ld_Vx_dt(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    chip8->V[x] = chip8->delay_timer;
    chip8->PC += 2;
}

/*
 * Wait for a key press, store the value of the key in Vx.
 * All execution stops until a key is pressed, then the value of that key is stored in Vx.
 */
void ld_Vx_key(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    
    for(int i = 0; i < 16; i++) {
        if(chip8->key[i] == 1) {
            chip8->V[x] = i;
            chip8->is_key_pressed = 1;
        }
    }

    if(!chip8->is_key_pressed) {
        return;
    }

    chip8->PC += 2;
}

/*
 * Set delay timer = Vx.
 * DT is set equal to the value of Vx.
 */
void ld_dt_Vx(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    chip8->delay_timer = chip8->V[x];
    chip8->PC += 2;
}

/*
 * Set sound timer = Vx.
 * ST is set equal to the value of Vx.
 */
void ld_st_Vx(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    chip8->sound_timer = chip8->V[x];
    chip8->PC += 2;
}

/*
 * Set I = I + Vx.
 * The values of I and Vx are added, and the results are stored in I.
 */
void add_i_Vx(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    chip8->I += chip8->V[x];
    chip8->PC += 2;
}

/*
 * Set I = location of sprite for digit Vx.
 * The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx.
 */
void ld_F_Vx(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    chip8->I = chip8->V[x] * 5;
    chip8->PC += 2;
}

/*
 * Store BCD representation of Vx in memory locations I, I+1, and I+2.
 * The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I,
 * the tens digit at location I+1, and the ones digit at location I+2.
 */
void ld_bcd_Vx(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    chip8->memory[chip8->I] = chip8->V[x] / 100;
    chip8->memory[chip8->I + 1] = (chip8->V[x] / 10) % 10;
    chip8->memory[chip8->I + 2] = (chip8->V[x] % 100) % 10;
    chip8->PC += 2;
}

/*
 * Store registers V0 through Vx in memory starting at location I.
 * The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
 */
void ld_regs_Vx(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    for(unsigned char i = 0; i <= x; i++) {
        chip8->memory[chip8->I + i] = chip8->V[i]; 
    }
    chip8->PC += 2;
}

/*
 * Read registers V0 through Vx from memory starting at location I.
 * The interpreter reads values from memory starting at location I into registers V0 through Vx.
 */
void ld_Vx_regs(Chip8 *chip8) {
    unsigned char x = (chip8->opcode & 0x0F00) >> 8;
    for(unsigned char i = 0; i <= x; i++) {
        chip8->V[i] = chip8->memory[chip8->I + i]; 
    }
    chip8->PC += 2;
}