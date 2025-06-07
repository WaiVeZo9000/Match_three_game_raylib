#include <raylib.h>
#include <stdio.h>

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Match Three Game");

    SetTargetFPS(60); // Set the game to run at 60 frames per second

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update your game logic here

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Welcome to Match Three Game!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}