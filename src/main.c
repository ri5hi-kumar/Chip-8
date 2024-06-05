#include "raylib.h"
#include "chip8.h"
#include <stdio.h>

Chip8 chip8;

int main()
{
    int const WINDOW_HEIGHT = 640;
    int const WINDOW_WIDTH = 1280;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CHIP-8");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // TODO: Update your variables here

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}