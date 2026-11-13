/**
 * @file main.c
 * @brief Ponto de entrada e loop principal do jogo.
 * @details Inicializa a janela, carrega os recursos globais e executa o game loop.
 *          O fluxo de estados segue a máquina definida em game_state.h:
 *
 *          STATE_MENU → (confirmado) → STATE_INTERFACE (TELA_INSTRUCOES)
 *          STATE_INTERFACE (instruções) → (ENTER) → STATE_PLAYING
 *          STATE_PLAYING ↔ STATE_INTERFACE  (em uma interface o jogo e o cronômetro pausam)
 *          STATE_PLAYING → (terminou) → STATE_WIN | STATE_GAME_OVER
 *          STATE_WIN | STATE_GAME_OVER → (ENTER) → STATE_MENU
 */

#include <raylib.h>
#include "core/game_state.h"
#include "core/resources.h"
#include "database/database.h"
#include "graphics/ui.h"
#include "scenes/gameplay.h"
#include "scenes/menu.h"
#include <string.h>

/**
 * @brief Escolhe qual música de fundo deve tocar em cada estado do jogo.
 * @param resources Ponteiro para os recursos globais (contém as músicas).
 * @param state     Estado atual do jogo.
 * @return Ponteiro para a música do estado, ou NULL se o estado não tem música.
 */
static Music *MusicaDoEstado(ResourcesBody *resources, GameState state) {
    if (state == STATE_MENU) return &resources->musica_menu;
    if (state == STATE_PLAYING || state == STATE_INTERFACE) return &resources->musica_gameplay;
    return NULL;   // STATE_WIN e STATE_GAME_OVER: silêncio
}

int main(void)
{
    InitWindow(900, 450, "Oráculo");
    InitAudioDevice();   // antes do ResourcesInit, senão as músicas não carregam
    SetExitKey(KEY_NULL); // desativa o ESC nativo do Raylib
    ToggleFullscreen();
    SetTargetFPS(60);

    int scream_width = GetScreenWidth();
    int scream_height = GetScreenHeight();

    // ── Recursos globais ──────────────────────────────────────────────────────
    ResourcesBody resources;
    ResourcesInit(&resources);

    // ── Menu ──────────────────────────────────────────────────────────────────
    MenuBody menu;
    MenuInit(&menu);

    // ── Estado do jogo ────────────────────────────────────────────────────────
    GameState state = STATE_MENU;
    Music *musica_atual = NULL;   // música tocando agora (NULL = nenhuma)

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
                gameplay.interface_atual = TELA_INSTRUCOES; 
                state = STATE_INTERFACE;
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
                    menu.nickname_input[0] = '\0';  
                    menu.nickname_len = 0;           
                    menu.nome_repetido = false; 
                    entrada.tempo = gameplay.tempo_conclusao;
                    // DataInsert retorna true só se a entrada entrou no top DATABASE_MAX_ENTRIES
                    if (DataInsert(resources.ranking, &resources.ranking_count, entrada)) {
                        DataSave(resources.ranking, resources.ranking_count);
                    }
                }else if (state == STATE_GAME_OVER) {
                    menu.nickname_input[0] = '\0';  
                    menu.nickname_len = 0;           
                    menu.nome_repetido = false;
                }
                GameplayUnload(&gameplay);
                GameplayInit(&gameplay);
                state = STATE_MENU;
            }
        }

        // -- Música: troca só quando o estado pede uma música diferente --
        Music *musica_desejada = MusicaDoEstado(&resources, state);
        if (musica_desejada != musica_atual) {
            if (musica_atual)     StopMusicStream(*musica_atual);
            if (musica_desejada)  PlayMusicStream(*musica_desejada);
            musica_atual = musica_desejada;
        }
        if (musica_atual) UpdateMusicStream(*musica_atual);   // obrigatório todo frame

        // -- Draw --
        BeginDrawing();
            ClearBackground(BLACK);

            if (state == STATE_MENU) {
                MenuDraw(&menu, &resources, scream_width, scream_height);

            } else if (state == STATE_PLAYING || state == STATE_INTERFACE) {
                GameplayDraw(&gameplay, &state, &resources, scream_width, scream_height);

            } else if (state == STATE_WIN) {
                GameplayDraw(&gameplay, &state, &resources, scream_width, scream_height);  // fundo da partida por baixo
                DrawRectangle(0, 0, scream_width, scream_height, (Color){ 0, 0, 0, 180 });
                DrawText("Missao cumprida!", scream_width/2 - 150, scream_height/2 - 30, 32, GREEN);
                DrawText("ENTER para voltar ao menu", scream_width/2 - 175, scream_height/2 + 20, 20, WHITE);

            } else if (state == STATE_GAME_OVER) {
                GameplayDraw(&gameplay, &state, &resources, scream_width, scream_height);  // fundo da partida por baixo
                DrawRectangle(0, 0, scream_width, scream_height, (Color){ 0, 0, 0, 200 });
                DrawText("Game Over", scream_width/2 - 100, scream_height/2 - 30, 40, RED);
                DrawText("ENTER para voltar ao menu", scream_width/2 - 175, scream_height/2 + 20, 20, WHITE);
            }

        EndDrawing();
    }

    // ── Unload ────────────────────────────────────────────────────────────────────
    MenuUnload(&menu);
    GameplayUnload(&gameplay);
    ResourcesUnload(&resources);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}