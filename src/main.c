#include <raylib.h>
#include "graphics/Animation.h"
#include "entities/player.h"
#include "map/map.h"
#include "core/resources.h"
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

int main(void)
{
    InitWindow(900, 450, "Oráculo");
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

    // ── Gameplay (inicializado aqui, mas só usado após sair do menu) ──────────
    PlayerBody player;
    PlayerInit(&player);

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
                menu.confirmado = false; // reseta para poder voltar ao menu depois
                state = STATE_GAMEPLAY;
            }

        } else {

            if (IsKeyPressed(KEY_M)) manual_aberto = !manual_aberto; // PROVISÓRIO

            if (!manual_aberto) {

                if (state == STATE_GAMEPLAY) {
                    if (IsKeyPressed(KEY_R)) PLayerReset(&player);
                    PlayerUpdateAction(&player);

                    // PROVISÓRIO
                    perto_do_notebook = CheckCollisionPointRec(player.position, ZONA_NOTEBOOK);
                    if (perto_do_notebook && IsKeyPressed(KEY_I)) {
                        state   = STATE_MENU_RELATORIO;
                        selecao = 0;
                    }

                } else if (state == STATE_MENU_RELATORIO) {
                    // PROVISÓRIO
                    if (IsKeyPressed(KEY_I))    state = STATE_GAMEPLAY;
                    if (IsKeyPressed(KEY_UP))   selecao = (selecao - 1 + 3) % 3;
                    if (IsKeyPressed(KEY_DOWN)) selecao = (selecao + 1) % 3;
                    if (IsKeyPressed(KEY_ENTER))
                        state = (selecao == RELATORIO_CORRETO) ? STATE_VITORIA : STATE_GAME_OVER;

                } else if (state == STATE_VITORIA) {
                    // PROVISÓRIO
                    if (IsKeyPressed(KEY_ENTER)) {
                        PLayerReset(&player);
                        state = STATE_GAMEPLAY;
                    }

                } else if (state == STATE_GAME_OVER) {
                    // PROVISÓRIO
                    if (IsKeyPressed(KEY_ENTER)) {
                        PLayerReset(&player);
                        state   = STATE_GAMEPLAY;
                        selecao = 0;
                    }
                }
            }
        }

        // ── Draw ──────────────────────────────────────────────────────────────
        BeginDrawing();
            ClearBackground(BLACK);

            if (state == STATE_MENU) {

                MenuDraw(&menu, &resources, sw, sh);

            } else {

                // PROVISÓRIO — gameplay e subtelas
                MapDraw(&map);
                PlayerDraw(&player);

                DrawText(TextFormat("pos: %.0f, %.0f", player.position.x, player.position.y), 10, 30, 16, WHITE); // DEBUG

                if (state == STATE_GAMEPLAY && perto_do_notebook) {
                    const char *dica = "[I] Inspecionar notebook";
                    int font_size = 24;
                    int texto_w   = MeasureText(dica, font_size);
                    int pad = 10;
                    int bx  = (int)player.position.x - 20;
                    int by  = (int)player.position.y - 50;
                    DrawRectangle(bx - pad, by - pad, texto_w + pad*2, font_size + pad*2, (Color){ 0, 0, 0, 180 });
                    DrawText(dica, bx, by, font_size, YELLOW);
                }

                if (state == STATE_MENU_RELATORIO) {
                    DrawRectangle(0, 0, sw, sh, (Color){ 0, 0, 0, 170 });
                    int px = sw / 2 - 460, py = 10;
                    int pw = 920, ph = sh - 20;
                    DrawRectangleRounded     ((Rectangle){ px, py, pw, ph }, 0.06f, 8, (Color){ 12, 18, 40, 245 });
                    DrawRectangleRoundedLines((Rectangle){ px, py, pw, ph }, 0.06f, 8, (Color){ 80, 120, 220, 255 });
                    DrawText("ARQUIVOS DO ORACULO", px + 24, py + 18, 28, WHITE);
                    DrawText("SETA CIMA/BAIXO   ENTER seleciona   I fecha", px + 24, py + 52, 16, (Color){ 130, 130, 180, 255 });

                    for (int i = 0; i < 3; i++) {
                        bool sel = (i == selecao);
                        int cx = px + 16;
                        int cy = py + 140 + i * 175;
                        int cw = pw - 32;
                        int ch = 165;
                        Color fundo   = sel ? (Color){ 30, 55, 120, 220 } : (Color){ 20, 28, 55, 200 };
                        Color borda   = sel ? (Color){ 100, 160, 255, 255 } : (Color){ 50, 70, 120, 180 };
                        Color c_titulo = sel ? YELLOW : WHITE;
                        Color c_meta   = (Color){ 120, 180, 255, 255 };
                        Color c_texto  = sel ? (Color){ 220, 220, 180, 255 } : (Color){ 160, 165, 185, 255 };
                        DrawRectangleRounded     ((Rectangle){ cx, cy, cw, ch }, 0.08f, 8, fundo);
                        DrawRectangleRoundedLines((Rectangle){ cx, cy, cw, ch }, 0.08f, 8, borda);
                        DrawText(sel ? ">" : " ",         cx + 10, cy + 12, 30, YELLOW);
                        DrawText(relatorios[i].titulo,    cx + 40, cy + 12, 28, c_titulo);
                        DrawText(relatorios[i].meta,      cx + 40, cy + 50, 20, c_meta);
                        DrawText(relatorios[i].documento, cx + 40, cy + 75, 20, c_meta);
                        for (int l = 0; relatorios[i].sintese[l] != 0; l++)
                            DrawText(relatorios[i].sintese[l], cx + 40, cy + 105 + l * 22, 19, c_texto);
                    }
                }

                if (state == STATE_VITORIA) {
                    DrawRectangle(0, 0, sw, sh, (Color){ 0, 0, 0, 180 });
                    DrawText("Relatorio correto enviado!", sw/2 - 195, sh/2 - 30, 28, GREEN);
                    DrawText("ENTER para continuar",      sw/2 - 115, sh/2 + 20,  20, WHITE);
                }

                if (state == STATE_GAME_OVER) {
                    DrawRectangle(0, 0, sw, sh, (Color){ 0, 0, 0, 200 });
                    DrawText("Relatorio incorreto.",              sw/2 - 155, sh/2 - 50, 32, RED);
                    DrawText("Os dados nao puderam ser verificados.", sw/2 - 220, sh/2,  20, (Color){ 200, 80, 80, 255 });
                    DrawText("ENTER para tentar novamente",       sw/2 - 175, sh/2 + 50, 20, WHITE);
                }

                if (manual_aberto) {
                    float escala_x = (float)sw / manual.width;
                    float escala_y = (float)sh / manual.height;
                    float escala   = (escala_x < escala_y) ? escala_x : escala_y;
                    float dest_w   = manual.width  * escala;
                    float dest_h   = manual.height * escala;
                    float dest_x   = (sw - dest_w) / 2.0f;
                    float dest_y   = (sh - dest_h) / 2.0f;
                    DrawRectangle(0, 0, sw, sh, (Color){ 0, 0, 0, 200 });
                    DrawTexturePro(manual,
                        (Rectangle){ 0, 0, (float)manual.width, (float)manual.height },
                        (Rectangle){ dest_x, dest_y, dest_w, dest_h },
                        (Vector2){ 0, 0 }, 0.0f, WHITE);
                    DrawText("M - Fechar", 10, 10, 18, YELLOW);
                } else {
                    DrawText("M - Manual", 10, 10, 18, WHITE);
                }
                // fim provisório
            }

        EndDrawing();
    }

    MenuUnload(&menu);
    UnloadTexture(manual); // PROVISÓRIO
    PlayerUnload(&player);
    MapUnload(&map);       // PROVISÓRIO
    ResourcesUnload(&resources);
    CloseWindow();
    return 0;
}