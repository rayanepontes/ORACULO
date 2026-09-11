#include "raylib.h"

void iniciar_tela()
{
   
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Oráculo - o jogo");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {   
        BeginDrawing();

            ClearBackground(BLACK);

            DrawText("Testando aqui se to escrevendo mesmo\nColoquei verde pra me sentir um hacker", 190, 200, 20, GREEN);

        EndDrawing();
    }   
    

    CloseWindow();
}