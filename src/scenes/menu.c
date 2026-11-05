#include <raylib.h>
#include "scenes/menu.h"

// ─── Funções internas ─────────────────────────────────────────────────────────

/** @brief Carrega as texturas de fundo de cada subtela.
 * @param self Ponteiro para o MenuBody.
 */
static void InitBackgrounds(MenuBody *self) {
    self->backgrounds[TELA_PRINCIPAL] = LoadTexture("assets/texture/menu/tela_principal.png");
    self->backgrounds[TELA_NOME]      = LoadTexture("assets/texture/menu/tela_nome.png");
    self->backgrounds[TELA_RANKING]   = LoadTexture("assets/texture/menu/frame 3.png");
}

// ─── Funções públicas ─────────────────────────────────────────────────────────

void MenuInit(MenuBody *self) {
    InitBackgrounds(self);
    self->tela_atual  = TELA_PRINCIPAL;
    self->opcao_atual = MENU_OPC_INICIAR;
    self->confirmado  = false;
}

void MenuUpdate(MenuBody *self) {
    switch (self->tela_atual) {
        case TELA_PRINCIPAL:
        
            if (IsKeyPressed(KEY_DOWN)) {
                self->opcao_atual = (self->opcao_atual + 1) % MENU_TOTAL_OPC;
            }

            if (IsKeyPressed(KEY_UP)) {
                self->opcao_atual = (self->opcao_atual - 1 + MENU_TOTAL_OPC) % MENU_TOTAL_OPC;
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (self->opcao_atual == MENU_OPC_RANKING){
                    self->tela_atual = TELA_RANKING;
                }else {
                    self->confirmado = true; // inicia partida
                }
            }
            break;

        case TELA_RANKING: 

            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                self->tela_atual = TELA_PRINCIPAL;
            }
            break;
    
        default:
            break;
    }
}

void MenuDraw(MenuBody *self, ResourcesBody *resources, int scream_width, int scream_height) {
    //lembrar depois de começar a adicionar as cores nos recursos globais (resources)
    Color amarelo = GetColor(0xEFB239FF);
    Color branco  = GetColor(0xE5E0EBFF);

    // Fundo da subtela ativa
    Texture2D bg = self->backgrounds[self->tela_atual];
    DrawTexturePro(
        bg,
        (Rectangle){ 0, 0, (float)bg.width, (float)bg.height },
        (Rectangle){ 0, 0, (float)scream_width, (float)scream_height },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );

    switch (self->tela_atual) {

        case TELA_PRINCIPAL: {
            const char *labels[MENU_TOTAL_OPC] = { "INICIAR PARTIDA", "RANKING" };

            for (int i = 0; i < MENU_TOTAL_OPC; i++) {
                bool  is_select = (i == self->opcao_atual);
                Color cor  = is_select ? amarelo : branco;
                float size = is_select ? 72.0f : 60.0f;

                Vector2 dimension = MeasureTextEx(resources->fonte_principal, labels[i], size, 1);
                Vector2 position  = {
                    (scream_width  / 2.0f) - (dimension.x / 2.0f),
                    (scream_height * 0.55f) + (i * 90.0f)
                };

                DrawTextEx(resources->fonte_principal, labels[i], position, size, 1, cor);
            }
            break;
        }

        case TELA_RANKING: {
            // implementar quando o sistema de ranking existir
            Vector2 position = {
                (scream_width / 2.0f) - 150.0f,
                scream_height / 2.0f
            };
            DrawTextEx(resources->fonte_principal, "RANKING EM BREVE", position, 60.0f, 1, WHITE);

            Vector2 position_voltar = {
                (scream_width / 2.0f) - 180.0f,
                scream_height * 0.75f
            };
            DrawTextEx(resources->fonte_principal, "ENTER/ESC - VOLTAR", position_voltar, 40.0f, 1, GRAY);
            break;
        }

        case TELA_NOME: {
            // implementar quando o sistema de nome existir
            Vector2 position = {
                (scream_width / 2.0f) - 120.0f,
                scream_height / 2.0f
            };
            DrawTextEx(resources->fonte_principal, "NOME EM BREVE", position, 60.0f, 1, WHITE);
            break;
        }

        default:
            break;
    }
}

void MenuUnload(MenuBody *self) {
    for (int i = 0; i < TELA_TOTAL; i++) {
        UnloadTexture(self->backgrounds[i]);
    }
    // fonte não é descarregada aqui — pertence ao ResourcesBody
}