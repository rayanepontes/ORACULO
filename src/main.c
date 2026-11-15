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

// PROVISÓRIO — entrega 12/10
#include <stddef.h>

typedef enum { STATE_MENU, STATE_GAMEPLAY, STATE_MENU_RELATORIO, STATE_VITORIA, STATE_GAME_OVER } GameState;

static const Rectangle ZONA_NOTEBOOK = { 1200.0f, 100.0f, 100.0f, 100.0f };
static const int RELATORIO_CORRETO = 1;

typedef struct {
    const char *titulo;
    const char *meta;
    const char *documento;
    const char *sintese[4];
} Relatorio;

static const Relatorio relatorios[3] = {
    { "RELATORIO A", "Confianca: 99%%  |  Temperatura: 0.8", "Documento: DOC-2010",
      { "O ORACULO apresenta informacoes sobre um procedimento",
        "interno da NEXUS Labs, mas os dados citados precisam ser",
        "comparados com outros registros para confirmar autenticidade.", 0 } },
    { "RELATORIO B", "Confianca: 99%%  |  Temperatura: 0.2", "Documento: DOC-2045",
      { "O ORACULO apresenta uma conclusao baseada em registros",
        "oficiais, indicando que as informacoes foram verificadas",
        "nos arquivos da NEXUS Labs.", 0 } },
    { "RELATORIO C", "Confianca: 99%%  |  Temperatura: 0.9", "Documento: DOC-1984",
      { "O ORACULO cita um suposto registro historico da NEXUS Labs,",
        "datado de 1984, que descreve acontecimentos relacionados",
        "as operacoes do laboratorio.", 0 } },
};
// fim provisório

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

    Texture2D manual   = LoadTexture("assets/texture/manual_jogador/foto_cap3.png"); // PROVISÓRIO
    bool manual_aberto = false;

    MapBody map; // PROVISÓRIO
    MapInit(&map, MAP_CROP, "assets/texture/sprites_teste/mapas_temporarios/Game Boy Advance - Pokemon FireRed.png");
    MapSetCrop(&map,
        (Rectangle){ 8.0f, 25.0f, 239.0f, 158.0f },
        (Rectangle){ 220.0f, 100.0f, 239*6.0f, 158*6.0f }
    );

    GameState state             = STATE_MENU;
    int       selecao           = 0;           // PROVISÓRIO
    bool      perto_do_notebook = false;       // PROVISÓRIO

    while (!WindowShouldClose())
    {
        // ── Update ────────────────────────────────────────────────────────────
        if (state == STATE_MENU) {

            MenuUpdate(&menu);

            if (menu.confirmado) {
                menu.confirmado = false;
                strncpy(gameplay.nickname, menu.nickname_input, 6);
                gameplay.nickname[6] = '\0';
                menu.tela_atual = TELA_PRINCIPAL;
                gameplay.interface_atual = TELA_INSTRUCOES; 
                state = STATE_INTERFACE;
            }

        } else {

            if (IsKeyPressed(KEY_M)) manual_aberto = !manual_aberto; // PROVISÓRIO

            if (!manual_aberto) {

                if (state == STATE_GAMEPLAY) {
                    if (IsKeyPressed(KEY_R)) PLayerReset(&player);
                    PlayerUpdateAction(&player);

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

    MenuUnload(&menu);
    UnloadTexture(manual); // PROVISÓRIO
    PlayerUnload(&player);
    MapUnload(&map);       // PROVISÓRIO
    ResourcesUnload(&resources);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}