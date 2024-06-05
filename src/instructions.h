#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "chip8.h";

/*
    nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
    n or nibble - A 4-bit value, the lowest 4 bits of the instruction
    x - A 4-bit value, the lower 4 bits of the high byte of the instruction
    y - A 4-bit value, the upper 4 bits of the low byte of the instruction
    kk or byte - An 8-bit value, the lowest 8 bits of the instruction
*/

void cls(Chip8 *chip8);             // (00E0) clear the screen
void ret(Chip8 *chip8);             // (00EE) return from a subroutine
void jmp(Chip8 *chip8);             // (1nnn) jump to location nnn
void call(Chip8 *chip8);            // (2nnn) call subroutine at nnn 
void se_Vx_kk(Chip8 *chip8);        // (3xkk) skip next instruction if Vx = kk
void sne_Vx_kk(Chip8 *chip8);       // (4xkk) skip next instruction if Vx != kk
void se_Vx_Vy(Chip8 *chip8);        // (5xy0) skip next instruction if Vx = Vy
void ld_Vx(Chip8 *chip8);           // (6xkk) set Vx = kk
void add_Vx_imm(Chip8 *chip8);          // (7xkk) set Vx = Vx + kk
void ld_Vx_Vy(Chip8 *chip8);        // (8xy0) set Vx = Vy
void or_Vx_Vy(Chip8 *chip8);        // (8xy1) set Vx = Vx OR Vy
void and_Vx_Vy(Chip8 *chip8);       // (8xy2) set Vx = Vx AND Vy
void xor_Vx_Vy(Chip8 *chip8);       // (8xy3) set Vx = Vx XOR Vy
void add_Vx_Vy(Chip8 *chip8);       // (8xy4) set Vx = Vx + Vy, set VF = carry
void sub_Vx_Vy(Chip8 *chip8);       // (8xy5) set Vx = Vx - Vy, set VF = NOT borrow
void shr(Chip8 *chip8);             // (8xy6) set Vx = Vx SHR 1
void subn_Vx_Vy(Chip8 *chip8);      // (8xy7) set Vx = Vy - Vx, set VF = NOT borrow
void shl(Chip8 *chip8);             // (8xyE) set Vx = Vx SHL 1
void sne_Vx_Vy(Chip8 *chip8);       // (9xy0) skip next instruction if Vx != Vy
void ldi(Chip8 *chip8);             // (Annn) set I = nnn
void jmp_V0(Chip8 *chip8);          // (Bnnn) jump to location nnn + V0
void rnd(Chip8 *chip8);             // (Cxkk) set Vx = random byte AND kk
void drw(Chip8 *chip8);             // (Dxyn) display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
void skp(Chip8 *chip8);             // (Ex9E) skip next instruction if key with the value of Vx is pressed
void sknp(Chip8 *chip8);            // (ExA1) skip next instruction if key with the value of Vx is not pressed
void ld_Vx_dt(Chip8 *chip8);        // (Fx07) set Vx = delay timer value
void ld_Vx_key(Chip8 *chip8);       // (Fx0A) wait for a key press, store the value of the key in Vx
void ld_dt_Vx(Chip8 *chip8);        // (Fx15) set delay timer = Vx
void ld_st_Vx(Chip8 *chip8);        // (Fx18) set sound timer = Vx
void add_i_Vx(Chip8 *chip8);        // (Fx1E) set I = I + Vx
void ld_F_Vx(Chip8 *chip8);         // (Fx29) set I = location of sprite for digit Vx
void ld_bcd_Vx(Chip8 *chip8);       // (Fx33) store BCD representation of Vx in memory locations I, I+1, and I+2
void ld_regs_Vx(Chip8 *chip8);       // (Fx55) store registers V0 through Vx in memory starting at location I
void ld_Vx_regs(Chip8 *chip8);       // (Fx65) read registers V0 through Vx from memory starting at location I

#endif