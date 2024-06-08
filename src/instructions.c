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
    for(int i = 0; i < 32; i++) {
        for(int j = 0; j < 64; j++) {
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
    chip8->V[0xF] = chip8->V[x] & 0b10000000 == 1 ? 1 : 0;
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

void drw(Chip8 *chip8) {

}

void skp(Chip8 *chip8) {

}

void sknp(Chip8 *chip8) {

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

void ld_Vx_key(Chip8 *chip8) {

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

void ld_F_Vx(Chip8 *chip8) {

}

void ld_bcd_Vx(Chip8 *chip8) {

}

void ld_regs_Vx(Chip8 *chip8) {

}

void ld_Vx_regs(Chip8 *chip8) {
    
}