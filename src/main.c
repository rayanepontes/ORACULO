/**
 * @file main.c
 * @brief Ponto de entrada e loop principal do jogo.
 * @details Inicializa a janela, carrega os recursos globais e executa o game loop.
 *          O fluxo de estados segue a máquina definida em game_state.h:
 *
 *          STATE_MENU → (confirmado) → STATE_PLAYING
 *          STATE_PLAYING ↔ STATE_INTERFACE  (Se tiver em uma interface o jogo não atualiza, exceto o tempo)
 *          STATE_PLAYING → (terminou) → STATE_WIN | STATE_GAME_OVER
 *          STATE_WIN | STATE_GAME_OVER → (ENTER) → STATE_MENU
 */

#include <raylib.h>
#include "core/game_state.h"
#include "core/resources.h"
#include "database/database.h"
#include "entities/player.h"
#include "graphics/Animation.h"
#include "graphics/ui.h"
#include "map/map.h"
#include "scenes/gameplay.h"
#include "scenes/menu.h"
#include <string.h>

int main(void)
{
    InitWindow(900, 450, "Oráculo");
    SetExitKey(KEY_NULL); // desativa o ESC nativo do Raylib
    ToggleFullscreen();
    SetTargetFPS(60);

    int scream_wigth = GetScreenWidth();
    int scream_height = GetScreenHeight();

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

            // jogador confirmou o nome: transfere nickname e inicia a partida
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
                menu.nickname_input[0] = '\0';  
                menu.nickname_len = 0;           
                menu.nome_repetido = false;      
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
                    // DataInsert retorna true só se a entrada entrou no top DATABASE_MAX_ENTRIES
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
                MenuDraw(&menu, &resources, scream_wigth, scream_height);

            } else if (state == STATE_PLAYING || state == STATE_INTERFACE) {
                GameplayDraw(&gameplay, &state, &resources, scream_wigth, scream_height);

            } else if (state == STATE_WIN) {
                GameplayDraw(&gameplay, &state, &resources, scream_wigth, scream_height);  // fundo da partida por baixo
                DrawRectangle(0, 0, scream_wigth, scream_height, (Color){ 0, 0, 0, 180 });
                DrawText("Missao cumprida!", scream_wigth/2 - 150, scream_height/2 - 30, 32, GREEN);
                DrawText("ENTER para voltar ao menu", scream_wigth/2 - 175, scream_height/2 + 20, 20, WHITE);

            } else if (state == STATE_GAME_OVER) {
                GameplayDraw(&gameplay, &state, &resources, scream_wigth, scream_height);  // fundo da partida por baixo
                DrawRectangle(0, 0, scream_wigth, scream_height, (Color){ 0, 0, 0, 200 });
                DrawText("Game Over", scream_wigth/2 - 100, scream_height/2 - 30, 40, RED);
                DrawText("ENTER para voltar ao menu", scream_wigth/2 - 175, scream_height/2 + 20, 20, WHITE);
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