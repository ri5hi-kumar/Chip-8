#include "raylib.h"
#include "chip8.h"
#include <stdio.h>

Chip8 chip8;
char *rom_file = "./roms/IBM_Logo.ch8";

int main()
{
    int const WINDOW_HEIGHT = 640;
    int const WINDOW_WIDTH = 1280;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CHIP-8");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    initialize_chip8(&chip8);
    load_rom(&chip8, rom_file);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        emulate_cycle(&chip8);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 32; y++) {
                if (chip8.gfx[x][y]) {
                    DrawRectangle(x * 20, y * 20, 20, 20, BLACK);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}