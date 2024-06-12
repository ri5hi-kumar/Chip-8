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
    fclose(rom);
}


void initialize_chip8(Chip8 *chip8) {
    chip8->PC = PC_START;
    chip8->SP = 0;
    chip8->opcode = 0;
    chip8->I = 0;
    chip8->sound_timer = 0;
    chip8->delay_timer = 0;
    chip8->draw_flag = 0;
    chip8->is_key_pressed = 0;

    // clearing the display
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 32; j++) {
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

    // clear registers
    for(int i = 0; i < 16; i++) {
        chip8->V[i] = 0; 
    }

    // keyboard setup
    for(int i = 0; i < 16; i++) {
        chip8->key[i] = 0;
    }

    // fonset loading
    for(int i = 0; i < 80; i++) {
        chip8->memory[i] = chip8_fontset[i];
    }
}

void emulate_cycle(Chip8 *chip8) {
    // fetch opcode from the rom memory which is at PC and PC + 1 (opcode is of 3 bytes)
    chip8->opcode = chip8->memory[chip8->PC] << 8 | chip8->memory[chip8-> PC + 1];

    // decode the opcode
    // CHIP-8’s index register and program counter can only address 12 bits (conveniently), which is 4096 addresses.
    switch(chip8->opcode & 0xF000) {
        case 0x0000:
            switch (chip8->opcode & 0x00FF) {
                case 0x00E0:
                    printf("CLS\n");
                    cls(chip8);
                    break;
                case 0x0EE:
                    printf("RET\n");
                    ret(chip8);
                    break;
                default:
                    break;
            }
            break;
        case 0x1000:
            printf("JP addr\n");
            jmp(chip8);
            break;
        case 0x2000:
            printf("CALL addr\n");
            call(chip8);
            break;
        case 0x3000:
            printf("SE Vx, byte\n");
            se_Vx_kk(chip8);
            break;
        case 0x4000:
            printf("SNE Vx, byte\n");
            sne_Vx_kk(chip8);
            break;
        case 0x5000:
            printf("SE Vx, Vy\n");
            se_Vx_Vy(chip8);
            break;
        case 0x6000:
            printf("LD Vx, byte\n");
            ld_Vx(chip8);
            break;
        case 0x7000:
            printf("ADD Vx, byte\n");
            add_Vx_kk(chip8);
            break;
        case 0x8000:
            switch (chip8->opcode & 0xF00F) {
                case 0x8001:
                    printf("OR Vx, Vy\n");
                    or_Vx_Vy(chip8);
                    break;
                case 0x8002:
                    printf("AND Vx, Vy\n");
                    and_Vx_Vy(chip8);
                    break;
                case 0x8003:
                    printf("XOR Vx, Vy\n");
                    xor_Vx_Vy(chip8);
                    break;
                case 0x8004:
                    printf("ADD Vx, Vy\n");
                    add_Vx_Vy(chip8);
                    break;
                case 0x8005:
                    printf("SUB Vx, Vy\n");
                    sub_Vx_Vy(chip8);
                    break;
                case 0x8006:
                    printf("SHR Vx {, Vy}\n");
                    shr(chip8);
                    break;
                case 0x8007:
                    printf("SUBN Vx, Vy\n");
                    subn_Vx_Vy(chip8);
                    break;
                case 0x800E:
                    printf("SHL Vx {, Vy}\n");
                    shl(chip8);
                    break;
                default:
                    break;
            }
            break;
        case 0x9000:
            printf("SNE Vx, Vy\n");
            sne_Vx_Vy(chip8);
            break;
        case 0xA000:
            printf("LD I, addr\n");
            ldi(chip8);
            break;
        case 0xB000:
            printf("JP V0, addr\n");
            jmp_V0(chip8);
            break;
        case 0xC000:
            printf("RND Vx, byte\n");
            rnd(chip8);
            break;
        case 0xD000:
            printf("DRW Vx, Vy, nibble\n");
            drw(chip8);
            break;
        case 0xE000:
            switch (chip8->opcode & 0xF0FF) {
                case 0xE09E:
                    printf("SKP Vx\n");
                    skp(chip8);
                    break;
                case 0xE0A1:
                    printf("SKNP Vx\n");
                    sknp(chip8);
                    break;
                default:
                    break;
            }
            break;
        case 0xF000:
            switch (chip8->opcode & 0xF0FF) {
                case 0xF007:
                    printf("LD Vx, DT\n");
                    ld_Vx_dt(chip8);
                    break;
                case 0xF00A:
                    printf("LD Vx, K\n");
                    ld_Vx_key(chip8);
                    break;
                case 0xF015:
                    printf("LD DT, Vx\n");
                    ld_dt_Vx(chip8);
                    break;
                case 0xF018:
                    printf("LD ST, Vx\n");
                    ld_st_Vx(chip8);
                    break;
                case 0xF01E:
                    printf("ADD I, Vx\n");
                    add_i_Vx(chip8);
                    break;
                case 0xF029:
                    printf("LD F, Vx\n");
                    ld_F_Vx(chip8);
                    break;
                case 0xF033:
                    printf("LD B, Vx\n");
                    ld_bcd_Vx(chip8);
                    break;
                case 0xF055:
                    printf("LD [I], Vx\n");
                    ld_regs_Vx(chip8);
                    break;
                case 0xF065:
                    printf("LD Vx, [I]\n");
                    ld_Vx_regs(chip8);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    // update timers
    if(chip8->delay_timer > 0) {
        --chip8->delay_timer;
    }

    if(chip8->sound_timer > 0) {
        if(chip8->sound_timer == 1) {
            printf("BEEP\n");
        }
        --chip8->sound_timer;
    }
}

/*
 *  Keypad                   Keyboard
 * +-+-+-+-+                +-+-+-+-+
 * |1|2|3|C|                |1|2|3|4|
 * +-+-+-+-+                +-+-+-+-+
 * |4|5|6|D|                |Q|W|E|R|
 * +-+-+-+-+       =>       +-+-+-+-+
 * |7|8|9|E|                |A|S|D|F|
 * +-+-+-+-+                +-+-+-+-+
 * |A|0|B|F|                |Z|X|C|V|
 * +-+-+-+-+                +-+-+-+-+
 * 
 * The above is the mapping for the chip8 hex keypad to keyboard
 */
void handle_input(Chip8 *chip8) {
    if(IsKeyDown(KEY_ONE)) {
        printf("1 key pressed\n");
        chip8->key[0x0] = 1;
    } else if(IsKeyUp(KEY_ONE)) {
        chip8->key[0x0] = 0;
    }

    if(IsKeyDown(KEY_TWO)) {
        printf("2 key pressed\n");
        chip8->key[0x1] = 1;
    } else if(IsKeyUp(KEY_TWO)) {
        chip8->key[0x1] = 0;
    }

    if(IsKeyDown(KEY_THREE)) {
        printf("3 key pressed\n");
        chip8->key[0x2] = 1;
    } else if(IsKeyUp(KEY_THREE)) {
        chip8->key[0x2] = 0;
    }

    if(IsKeyDown(KEY_FOUR)) {
        printf("4 key pressed\n");
        chip8->key[0x3] = 1;
    } else if(IsKeyUp(KEY_FOUR)) {
        chip8->key[0x3] = 0;
    }

    if(IsKeyDown(KEY_Q)) {
        printf("Q key pressed\n");
        chip8->key[0x4] = 1;
    } else if(IsKeyUp(KEY_Q)) {
        chip8->key[0x4] = 0;
    }

    if(IsKeyDown(KEY_W)) {
        printf("W key pressed\n");
        chip8->key[0x5] = 1;
    } else if(IsKeyUp(KEY_W)) {
        chip8->key[0x5] = 0;
    }

    if(IsKeyDown(KEY_E)) {
        printf("E key pressed\n");
        chip8->key[0x6] = 1;
    } else if(IsKeyUp(KEY_E)) {
        chip8->key[0x6] = 0;
    }

    if(IsKeyDown(KEY_R)) {
        printf("R key pressed\n");
        chip8->key[0x7] = 1;
    } else if(IsKeyUp(KEY_R)) {
        chip8->key[0x7] = 0;
    }

    if(IsKeyDown(KEY_A)) {
        printf("A key pressed\n");
        chip8->key[0x8] = 1;
    } else if(IsKeyUp(KEY_A)) {
        chip8->key[0x8] = 0;
    }

    if(IsKeyDown(KEY_S)) {
        printf("S key pressed\n");
        chip8->key[0x9] = 1;
    } else if(IsKeyUp(KEY_S)) {
        chip8->key[0x9] = 0;
    }

    if(IsKeyDown(KEY_D)) {
        printf("D key pressed\n");
        chip8->key[0xA] = 1;
    } else if(IsKeyUp(KEY_D)) {
        chip8->key[0xA] = 0;
    }

    if(IsKeyDown(KEY_F)) {
        printf("F key pressed\n");
        chip8->key[0xB] = 1;
    } else if(IsKeyUp(KEY_F)) {
        chip8->key[0xB] = 0;
    }

    if(IsKeyDown(KEY_Z)) {
        printf("Z key pressed\n");
        chip8->key[0xC] = 1;
    } else if(IsKeyUp(KEY_Z)) {
        chip8->key[0xC] = 0;
    }

    if(IsKeyDown(KEY_X)) {
        printf("X key pressed\n");
        chip8->key[0xD] = 1;
    } else if(IsKeyUp(KEY_X)) {
        chip8->key[0xD] = 0;
    }

    if(IsKeyDown(KEY_C)) {
        printf("C key pressed\n");
        chip8->key[0xE] = 1;
    } else if(IsKeyUp(KEY_C)) {
        chip8->key[0xE] = 0;
    }

    if(IsKeyDown(KEY_V)) {
        printf("V key pressed\n");
        chip8->key[0xF] = 1;
    } else if(IsKeyUp(KEY_V)) {
        chip8->key[0xF] = 0;
    }
}