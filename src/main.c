#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BOARD_SIZE 8 // Size of the game board (8x8)
#define TILE_SIZE 50 // Size of each tile in pixels
#define TILE_TYPES 5 // Number of different tile types

char tileTypes[TILE_TYPES] = {'@', '#', '&', '$', '%'};
char Board[BOARD_SIZE][BOARD_SIZE];
bool matched[BOARD_SIZE][BOARD_SIZE]; // store the matched value of the tiles

int score = 100;
Vector2 origin;

char random_tile(){
    return tileTypes[rand() % TILE_TYPES];
}

void init_board(){
    for (int y = 0; y < BOARD_SIZE; y++){
        for (int x = 0; x < BOARD_SIZE; x++){
            Board[y][x] = random_tile();
        }
    }
    int grid_width = BOARD_SIZE * TILE_SIZE;
    int grid_height = BOARD_SIZE * TILE_SIZE;

    // This will be the start of the board
    origin = (Vector2){
        (GetScreenWidth() - grid_width )/ 2,
        (GetScreenHeight() - grid_height) / 2
    };
}

void resolve_matches(){
    // Iterate the board columns by columns
    for (int x = 0 ; x < BOARD_SIZE ; x++){
        char tempColumn[BOARD_SIZE];
        int tempIndex = BOARD_SIZE - 1; // To track the temp column and fill it from bottom to top

        // Collect non-matched tiles from bottom to top
        for (int y = BOARD_SIZE - 1 ; y > 0 ; y--){
            if (!matched[y][x]){
                tempColumn[tempIndex--] = Board[y][x];
            }
        }

        // Fill the remaining top spots  in temp column with new random tiles
        while (tempIndex >= 0){
            tempColumn[tempIndex--] = random_tile();
        }

        // Copy the column back to the original 
        for (int i = 0 ; i < BOARD_SIZE ; i++){
            Board[i][x] = tempColumn[i] ;
        }
    }    
}

bool find_matches(){
    bool found = false;
    // draw a virtual board filled with matched properties set it all to false.
    for (int y = 0; y < BOARD_SIZE; y++){
        for (int x = 0; x < BOARD_SIZE; x++){
            matched[y][x] = false; 
        }
    }

    // check the horizontal tiles
    for (int y = 0; y < BOARD_SIZE; y++){
        for (int x = 0 ; x < BOARD_SIZE - 2; x++){
            char t = Board[y][x];
            if (t == Board[y][x + 1] && t == Board[y][x + 2]){
                matched[y][x] = matched[y][x + 1] = matched[y][x + 2] = true;

                score += 10;
                found = true;
            }
        }
    }    

    // check the vertical tiles
    for (int x = 0; x < BOARD_SIZE; x++){
        for (int y = 0 ; y < BOARD_SIZE - 2; y++){
            char t = Board[y][x];
            if (t == Board[y + 1][x] && t == Board[y + 2][x]){
            matched[y][x] = matched[y + 1][x] = matched[y + 2][x] = true;

                score += 10;
                found = true;
            }
        }
    }    

    return found;
}

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Initialize the mouse position
    Vector2 mousePos = {0, 0};
    Vector2 selectedTile = {-10, -10}; // To keep track of the selected tile

    InitWindow(screenWidth, screenHeight, "Match Three Game");

    SetTargetFPS(60); // Set the game to run at 60 frames per second

    Texture2D background = LoadTexture("assets/cool_background.jpg");
    
    if (background.id == 0){
        printf("Error in loading background.\n");
        return 1;
    }

    srand(time(NULL));

    init_board();
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update your game logic here
        mousePos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            int x = (mousePos.x - origin.x) / TILE_SIZE;
            int y = (mousePos.y - origin.y) / TILE_SIZE;
            if (x >= 0 && x <= BOARD_SIZE && y >= 0 && y <= BOARD_SIZE){
                selectedTile = (Vector2){x, y};
            }
        }
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        // Add the background images
        DrawTexturePro(background,
            (Rectangle){
                0,0, background.width, background.height
            },
            (Rectangle){
                0,0, GetScreenWidth(), GetScreenHeight()
            },
            (Vector2){
                0,0
            },
            0,
            WHITE
        );

        find_matches();

        resolve_matches();
        // Draw a 8x8 board
        for (int i = 0; i < BOARD_SIZE; i++){
            for (int j = 0; j < BOARD_SIZE; j++){
                Rectangle rec = {
                 origin.x + (i * TILE_SIZE),
                 origin.y + (j * TILE_SIZE),
                 TILE_SIZE,
                 TILE_SIZE   
                };

                DrawRectangleLinesEx(rec, 2,DARKBLUE);

                DrawTextEx(
                    GetFontDefault(),
                    TextFormat("%c", Board[i][j]),
                    (Vector2){
                        rec.x + 17, rec.y + 10},
                    20,
                    1,
                    matched[i][j] ? GREEN : WHITE);
            }
        }
        // Draw the selected tile
        DrawRectangleLinesEx((Rectangle){
          origin.x + (selectedTile.x * TILE_SIZE),
          origin.y + (selectedTile.y * TILE_SIZE),
          TILE_SIZE,
          TILE_SIZE  
        }, 2, RED);

        if (score == 0)
        {
            DrawText(TextFormat("Score : %d", score), 10, 10, 20, WHITE);
            /* code */
        }
        else {
            DrawText(TextFormat("Scores : %d", score), 10, 10, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(background);
    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}