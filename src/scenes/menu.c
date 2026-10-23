#include <raylib.h>
#include <stdbool.h>
#include "scenes/menu.h"

// ─── Funções internas ─────────────────────────────────────────────────────────

/** @brief Carrega as texturas de fundo de cada subtela.
 * @param self Ponteiro para o MenuBody.
 */
static void InitBackgrounds(MenuBody *self) {
    self->backgrounds[TELA_PRINCIPAL] = LoadTexture("assets/texture/menu/tela_principal.png");
    self->backgrounds[TELA_NOME]      = LoadTexture("assets/texture/menu/tela_principal.png");
    self->backgrounds[TELA_RANKING]   = LoadTexture("assets/texture/menu/frame 3.png");
    self->caixa_nome = LoadTexture("assets/texture/menu/Rectangle 122.png");
    self->nickname_input[0] = '\0';
    self->nickname_len = 0;
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
                    self->tela_atual = TELA_NOME;
                }
            }
            break;

        case TELA_NOME: {
            if (IsKeyPressed(KEY_ESCAPE)) {
                self->tela_atual = TELA_NOME;
                // limpa o que foi digitado ao voltar
                self->nickname_input[0] = '\0';
                self->nickname_len      = 0;
                break;
            }

            // apaga última letra com BACKSPACE
            if (IsKeyPressed(KEY_BACKSPACE) && self->nickname_len > 0) {
                self->nickname_len--;
                self->nickname_input[self->nickname_len] = '\0';
            }

            // confirma com ENTER (só se digitou pelo menos 1 char)
            if (IsKeyPressed(KEY_ENTER) && self->nickname_len > 0) {
                self->confirmado = true;
                break;
            }

            // lê letra digitada (GetCharPressed retorna 0 quando não há tecla)
            int ch = GetCharPressed();
            while (ch != 0) {
                // aceita letras e números, máximo 6 chars
                bool is_valid = (ch >= 'A' && ch <= 'Z')
                             || (ch >= 'a' && ch <= 'z')
                             || (ch >= '0' && ch <= '9');
                if (is_valid && self->nickname_len < 6) {
                    self->nickname_input[self->nickname_len] = (char)ch;
                    self->nickname_len++;
                    self->nickname_input[self->nickname_len] = '\0';
                }
                ch = GetCharPressed(); // pode ter mais de uma tecla no buffer
            }
            break;
        }    
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
                Color cor  = is_select ? resources->amarelo : resources->branco;
                float size = is_select ? 72.0f : 60.0f;   // tamanho: selecionado / normal

                Vector2 dimension = MeasureTextEx(resources->fonte_principal, labels[i], size, 1);
                Vector2 position  = {
                    (scream_width  / 2.0f) - (dimension.x / 2.0f),        // centralizado na tela
                    (scream_height * 0.55f) + (i * 90.0f)                  // altura inicial + espaço entre opções
                };

                DrawTextEx(resources->fonte_principal, labels[i], position, size, 1, cor);
            }
            break;
        }
        case TELA_NOME: {
            // --- label "Digite seu nome:" ---
            const char *label = "Digite seu nome:";
            float label_size  = 48.0f;                                     // tamanho do texto do label
            Vector2 label_dim = MeasureTextEx(resources->fonte_principal, label, label_size, 1);
            Vector2 label_pos = {
                (scream_width  / 2.0f) - (label_dim.x / 2.0f),            // centralizado na tela
                (scream_height * 0.62f) - label_dim.y - 12.0f             // logo acima da caixa (12px de margem)
            };
            DrawTextEx(resources->fonte_principal, label, label_pos, label_size, 1, resources->branco);

            // --- caixa de digitação ---
            float box_w = 520.0f;                                          // largura da caixa
            float box_h = 75.0f;                                           // altura da caixa
            float box_x = (scream_width  / 2.0f) - (box_w / 2.0f);       // centralizada na tela
            float box_y = (scream_height * 0.67f);                         // posição vertical (0.0 = topo, 1.0 = base)
            DrawTexturePro(
                self->caixa_nome,
                (Rectangle){ 0, 0, (float)self->caixa_nome.width, (float)self->caixa_nome.height },
                (Rectangle){ box_x, box_y, box_w, box_h },
                (Vector2){ 0, 0 },
                0.0f,
                WHITE
            );

            // --- slots de letras (underscores + caracteres digitados) ---
            float slot_size    = 52.0f;                                    // tamanho da fonte dos caracteres
            float slot_spacing = 72.0f;                                    // distância entre o início de cada slot
            float slots_total  = 6 * slot_spacing - (slot_spacing - slot_size); // largura ocupada pelos 6 slots juntos
            float slot_start_x = (scream_width / 2.0f) - (slots_total / 2.0f); // x do primeiro slot (centralizado)
            float slot_y       = box_y + (box_h / 2.0f) - (slot_size / 2.0f) - 6.0f;  // y centralizado dentro da caixa

            for (int i = 0; i < 6; i++) {
                float sx = slot_start_x + i * slot_spacing;               // x deste slot específico

                if (i < self->nickname_len) {
                    // slot preenchido: mostra a letra digitada
                    char letra[2] = { self->nickname_input[i], '\0' };
                    Vector2 letra_dim = MeasureTextEx(resources->fonte_principal, letra, slot_size, 1);
                    Vector2 letra_pos = {
                        sx + (slot_size / 2.0f) - (letra_dim.x / 2.0f),  // centralizada dentro do slot
                        slot_y
                    };
                    DrawTextEx(resources->fonte_principal, letra, letra_pos, slot_size, 1, resources->letra_caixa);
                } else {
                    // slot vazio: mostra underscore (cursor pisca no slot atual)
                    Vector2 under_dim = MeasureTextEx(resources->fonte_principal, "_", slot_size, 1);
                    Vector2 under_pos = {
                        sx + (slot_size / 2.0f) - (under_dim.x / 2.0f),  // centralizado dentro do slot
                        slot_y + 8.0f                                      // ajuste vertical do underscore
                    };
                    bool piscando = (i == self->nickname_len) && ((int)(GetTime() * 2) % 2 == 0); // 2x por segundo
                    Color under_cor = (i == self->nickname_len)
                        ? (piscando ? resources->amarelo : resources->lilas_escuro) // cursor: pisca entre amarelo e lilás
                        : resources->lilas_escuro;                                  // slots futuros: lilás fixo
                    DrawTextEx(resources->fonte_principal, "_", under_pos, slot_size, 1, under_cor);
                }
            }

            // --- instrução no rodapé ---
            const char *hint = "ENTER - confirmar   ESC - voltar";
            Vector2 hint_dim = MeasureTextEx(resources->fonte_principal, hint, 32.0f, 1); // tamanho do texto hint
            Vector2 hint_pos = {
                (scream_width  / 2.0f) - (hint_dim.x / 2.0f),            // centralizado na tela
                box_y + box_h + 24.0f                                      // 24px abaixo da caixa
            };
            DrawTextEx(resources->fonte_principal, hint, hint_pos, 32.0f, 1, resources->lilas_hint);
            break;
        }
        case TELA_RANKING: {
            Vector2 position = {
                (scream_width / 2.0f) - 150.0f,
                scream_height / 2.0f
            };
            DrawTextEx(resources->fonte_principal, "RANKING EM BREVE", position, 60.0f, 1, WHITE);

            Vector2 position_voltar = {
                (scream_width / 2.0f) - 180.0f,
                scream_height * 0.75f                                      // 75% da altura da tela
            };
            DrawTextEx(resources->fonte_principal, "ENTER/ESC - VOLTAR", position_voltar, 40.0f, 1, GRAY);
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
    UnloadTexture(self->caixa_nome);
    // fonte não é descarregada aqui — pertence ao ResourcesBody
}