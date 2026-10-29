#include <raylib.h>
#include "core/game_state.h"
#include "core/resources.h"
#include "database/database.h"
#include "entities/player.h"
#include "graphics/Animation.h"
#include "map/map.h"
#include "scenes/gameplay.h"
#include "scenes/menu.h"
#include <string.h>

int main(void)
{
    InitWindow(900, 450, "Oráculo");
    SetExitKey(KEY_NULL);
    ToggleFullscreen();
    SetTargetFPS(60);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // ── Recursos globais ──────────────────────────────────────────────────────
    ResourcesBody resources;
    ResourcesInit(&resources);

    // ── Menu ──────────────────────────────────────────────────────────────────
    MenuBody menu;
    MenuInit(&menu);

    // ── Estado do jogo ────────────────────────────────────────────────────────
    GameState state = STATE_MENU;

    // ── Gameplay  ─────────────────────────────────────────────────────────────
    GameplayBody gameplay;
    GameplayInit(&gameplay);

    // ── Game loop ─────────────────────────────────────────────────────────────────
    while (!WindowShouldClose() && !menu.sair) {

        // -- Update --
        if (state == STATE_MENU) {

            MenuUpdate(&menu, &resources);

            if (menu.confirmado) {
                menu.confirmado = false;
                strncpy(gameplay.nickname, menu.nickname_input, 6);
                gameplay.nickname[6] = '\0';
                menu.tela_atual = TELA_PRINCIPAL;
                state = STATE_PLAYING;
            }

        } else if (state == STATE_PLAYING || state == STATE_INTERFACE) {

            GameplayUpdate(&gameplay, &state);

            if (gameplay.voltar_menu) {
                gameplay.voltar_menu = false;
                GameplayUnload(&gameplay);
                GameplayInit(&gameplay);
                state = STATE_MENU;
            } else if (gameplay.terminou) {
                state = gameplay.venceu ? STATE_WIN : STATE_GAME_OVER;
            }

        } else if (state == STATE_WIN || state == STATE_GAME_OVER) {

            if (IsKeyPressed(KEY_ENTER)) {
                // salva no ranking se venceu
                if (state == STATE_WIN) {
                    GameData entrada;
                    strncpy(entrada.nickname, gameplay.nickname, 6);
                    entrada.nickname[6] = '\0';
                    entrada.score = gameplay.score;
                    if (DataInsert(resources.ranking, &resources.ranking_count, entrada)) {
                        DataSave(resources.ranking, resources.ranking_count);
                    }
                }
                GameplayUnload(&gameplay);
                GameplayInit(&gameplay);
                state = STATE_MENU;
            }
        }

        // -- Draw --
        BeginDrawing();
            ClearBackground(BLACK);

            if (state == STATE_MENU) {
                MenuDraw(&menu, &resources, sw, sh);

            } else if (state == STATE_PLAYING || state == STATE_INTERFACE) {
                GameplayDraw(&gameplay);

            } else if (state == STATE_WIN) {
                GameplayDraw(&gameplay);  // fundo da partida por baixo
                DrawRectangle(0, 0, sw, sh, (Color){ 0, 0, 0, 180 });
                DrawText("Missao cumprida!", sw/2 - 150, sh/2 - 30, 32, GREEN);
                DrawText("ENTER para voltar ao menu", sw/2 - 175, sh/2 + 20, 20, WHITE);

            } else if (state == STATE_GAME_OVER) {
                GameplayDraw(&gameplay);  // fundo da partida por baixo
                DrawRectangle(0, 0, sw, sh, (Color){ 0, 0, 0, 200 });
                DrawText("Game Over", sw/2 - 100, sh/2 - 30, 40, RED);
                DrawText("ENTER para voltar ao menu", sw/2 - 175, sh/2 + 20, 20, WHITE);
            }

        EndDrawing();
    }

    // ── Unload ────────────────────────────────────────────────────────────────────
    MenuUnload(&menu);
    GameplayUnload(&gameplay);
    ResourcesUnload(&resources);
    CloseWindow();
    return 0;
}