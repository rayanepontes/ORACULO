#include <raylib.h>
#include "graphics/Animation.h"
#include "entities/player.h"

int main(void)
{
    const int SCREEN_WIDTH = 900;
    const int SCREEN_HEIGHT = 450;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Oráculo");
        SetTargetFPS(60);

        PlayerBody player;
        PlayerInit(&player);

        while (!WindowShouldClose())
        {   
            // teste: pressionar R reseta o player como se fosse nova partida
            if (IsKeyDown(KEY_R)) PLayerReset(&player);

            PlayerUpdateAction(&player);

            BeginDrawing();

                ClearBackground(ORANGE);
                PlayerDraw(&player);

            EndDrawing();
        }   

        PlayerUnload(&player);
    CloseWindow();
    return 0;
}