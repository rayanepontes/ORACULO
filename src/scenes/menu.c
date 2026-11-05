#include <raylib.h>
#include <stdbool.h>
#include <ctype.h>
#include "scenes/menu.h"
#include "core/resources.h"
#include "graphics/ui.h"

// ─── Funções internas ─────────────────────────────────────────────────────────

/** @brief Carrega as texturas de fundo de cada subtela.
 * @param self Ponteiro para o MenuBody.
 */
static void InitBackgrounds(MenuBody *self) {
    self->backgrounds[TELA_PRINCIPAL] = LoadTexture("assets/texture/menu/tela_principal.png");
    self->backgrounds[TELA_NOME]      = LoadTexture("assets/texture/menu/tela_principal.png");
    self->backgrounds[TELA_RANKING]   = LoadTexture("assets/texture/menu/tela_vazia.png");
    self->backgrounds[TELA_CONFIRMA_SAIR] = LoadTexture("assets/texture/menu/tela_principal.png");
    self->caixa_nome = LoadTexture("assets/texture/menu/Rectangle 122.png");
}

static bool NomeJaExiste(const ResourcesBody *resources, const char *nome) {
    for (int i = 0; i < resources->ranking_count; i++) {
        const char *existente = resources->ranking[i].nickname;
        int j = 0;

        while (existente[j] != '\0' && nome[j] != '\0' &&
               tolower((unsigned char)existente[j]) == tolower((unsigned char)nome[j])) {
            j++;
        }

        // chegou ao fim das duas strings juntas = iguais ignorando maiúscula/minúscula
        if (existente[j] == '\0' && nome[j] == '\0') return true;
    }
    return false;
}

// ─── Funções públicas ─────────────────────────────────────────────────────────

void MenuInit(MenuBody *self) {
    InitBackgrounds(self);
    self->nickname_input[0] = '\0';
    self->nickname_len = 0;
    self->nome_repetido = false;
    self->tela_atual  = TELA_PRINCIPAL;
    self->opcao_atual = MENU_OPC_INICIAR;
    self->confirmado  = false;
    self->sair = false;
    self->opc_confirmacao = 0;
}

void MenuUpdate(MenuBody *self, const ResourcesBody *resources) {
    switch (self->tela_atual) {
        case TELA_PRINCIPAL:
            int confirmado = UiOpcaoUpdate((int*)&self->opcao_atual, MENU_TOTAL_OPC);
            if (confirmado == MENU_OPC_INICIAR) { self->tela_atual = TELA_NOME; }
            if (confirmado == MENU_OPC_RANKING) { self->tela_atual = TELA_RANKING; }
            if (confirmado == MENU_OPC_SAIR)    { self->tela_atual = TELA_CONFIRMA_SAIR; }
            break;

        case TELA_NOME: 
            if (IsKeyPressed(KEY_ESCAPE)) {
                // limpa o que foi digitado ao voltar
                self->tela_atual = TELA_PRINCIPAL;
                self->nickname_input[0] = '\0';
                self->nickname_len      = 0;
                break;
            }

            // apaga última letra com BACKSPACE
            if (IsKeyPressed(KEY_BACKSPACE) && self->nickname_len > 0) {
                self->nickname_len--;
                self->nickname_input[self->nickname_len] = '\0';
                self->nome_repetido = false;
            }

            if (IsKeyPressed(KEY_ENTER) && self->nickname_len > 0) {
                if (NomeJaExiste(resources, self->nickname_input)) {
                    self->nome_repetido = true;      // não confirma
                } else {
                    self->confirmado = true;
                }
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
                    self->nome_repetido = false;
                }
                ch = GetCharPressed(); // pode ter mais de uma tecla no buffer
            }
            break;
            
        case TELA_RANKING: 
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                self->tela_atual = TELA_PRINCIPAL;
            }
            break;

        case TELA_CONFIRMA_SAIR: {
            int confirmado = UiOpcaoUpdate(&self->opc_confirmacao, 2);
            if (confirmado == 0) {
                self->sair = true; // SIM
            } else if (confirmado == 1) {
                self->opc_confirmacao = 0;
                self->tela_atual = TELA_PRINCIPAL; // NAO
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                self->opc_confirmacao = 0;
                self->tela_atual = TELA_PRINCIPAL;
            }
            break;
        }

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
            const char *labels[MENU_TOTAL_OPC] = { "INICIAR PARTIDA", "RANKING", "SAIR" };

            UiOpcaoEstilo estilo = {
                .size_normal  = 60.0f,
                .size_select  = 72.0f,
                .cor_normal   = resources->branco,
                .cor_select   = resources->amarelo,
                .espacamento  = 90.0f
            };

            UiOpcaoDraw(resources->fonte_principal, labels, MENU_TOTAL_OPC,
                        self->opcao_atual, estilo, scream_height * 0.55f, scream_width);
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
            const char *instrucao = "ENTER - confirmar   ESC - voltar";
            Vector2 instrucao_dimension = MeasureTextEx(resources->fonte_principal, instrucao, 32.0f, 1); // tamanho do texto instrucao
            Vector2 instrucao_position = {
                (scream_width  / 2.0f) - (instrucao_dimension.x / 2.0f),            // centralizado na tela
                box_y + box_h + 24.0f                                      // 24px abaixo da caixa
            };
            DrawTextEx(resources->fonte_principal, instrucao, instrucao_position, 32.0f, 1, resources->lilas_claro);

            if (self->nome_repetido) {
                const char *erro = "Esse nome ja foi cadastrado!";
                Vector2 erro_dim = MeasureTextEx(resources->fonte_principal, erro, 36.0f, 1);
                Vector2 erro_pos = {
                    (scream_width / 2.0f) - (erro_dim.x / 2.0f),
                    label_pos.y + 60.0f
                };
                DrawTextEx(resources->fonte_principal, erro, erro_pos, 36.0f, 1, RED);
            }
            break;
        }
        case TELA_RANKING: {
            // --- título ---
            const char *titulo = "RANKING";
            float titulo_size  = 72.0f;                                        // tamanho do título
            Vector2 titulo_dim = MeasureTextEx(resources->fonte_principal, titulo, titulo_size, 1);
            Vector2 titulo_pos = {
                (scream_width / 2.0f) - (titulo_dim.x / 2.0f),                // centralizado
                scream_height * 0.22f                                           // posição vertical do título
            };
            DrawTextEx(resources->fonte_principal, titulo, titulo_pos, titulo_size, 1, resources->amarelo);

            // --- lista de entradas ---
            float entry_size    = 50.0f;                                       // tamanho da fonte de cada linha
            float entry_start_y = scream_height * 0.35f;                       // y da primeira entrada
            float entry_spacing = 40.0f;                                       // espaço vertical entre entradas

            // --- colunas (ajuste os valores x para centralizar no computador) ---
            float col_rank_x  = scream_width * 0.32f;   // coluna: número da posição
            float col_nome_x  = scream_width * 0.47f;   // coluna: nickname
            float col_pont_x  = scream_width * 0.65f;   // coluna: pontuação

            // --- cabeçalho ---
            float header_y    = entry_start_y - 48.0f;  // logo acima da primeira entrada
            float header_size = 36.0f;                  // tamanho da fonte do cabeçalho
            DrawTextEx(resources->fonte_principal, "RANK", (Vector2){ col_rank_x, header_y }, header_size, 1, resources->lilas_claro);
            DrawTextEx(resources->fonte_principal, "NAME", (Vector2){ col_nome_x, header_y }, header_size, 1, resources->lilas_claro);
            DrawTextEx(resources->fonte_principal, "SCORE",(Vector2){ col_pont_x, header_y }, header_size, 1, resources->lilas_claro);


            if (resources->ranking_count == 0) {
                const char *vazio = "Nenhuma partida registrada ainda.";
                Vector2 vazio_dim = MeasureTextEx(resources->fonte_principal, vazio, 40.0f, 1);
                Vector2 vazio_pos = {
                    (scream_width / 2.0f) - (vazio_dim.x / 2.0f),
                    scream_height * 0.45f
                };
                DrawTextEx(resources->fonte_principal, vazio, vazio_pos, 40.0f, 1, resources->lilas_claro);
            } else {
                // --- lista de entradas ---
                for (int i = 0; i < resources->ranking_count; i++) {
                    float y   = entry_start_y + i * entry_spacing;
                    Color cor = (i == 0) ? resources->amarelo : resources->branco;

                    DrawTextEx(resources->fonte_principal,
                        TextFormat("%d.", i + 1),
                        (Vector2){ col_rank_x, y }, entry_size, 1, cor);

                    DrawTextEx(resources->fonte_principal,
                        resources->ranking[i].nickname,
                        (Vector2){ col_nome_x, y }, entry_size, 1, cor);

                    DrawTextEx(resources->fonte_principal,
                        TextFormat("%d", resources->ranking[i].score),
                        (Vector2){ col_pont_x, y }, entry_size, 1, cor);
                }
            }

            // --- instrução no rodapé ---
            const char *instrucao = "ENTER / ESC - voltar";
            Vector2 instrucao_dimension = MeasureTextEx(resources->fonte_principal, instrucao, 32.0f, 1);
            Vector2 instrucao_position = {
                (scream_width / 2.0f) - (instrucao_dimension.x / 2.0f),                  // centralizado
                scream_height * 0.75f                                           // próximo ao rodapé
            };
            DrawTextEx(resources->fonte_principal, instrucao, instrucao_position, 32.0f, 1, resources->lilas_claro);
            break;
        }
        case TELA_CONFIRMA_SAIR: {
            const char *msg = "Deseja mesmo fechar o jogo?";
            Vector2 msg_dim = MeasureTextEx(resources->fonte_principal, msg, 56.0f, 1);
            DrawTextEx(resources->fonte_principal, msg,
                (Vector2){ (scream_width / 2.0f) - (msg_dim.x / 2.0f), scream_height * 0.55f },
                56.0f, 1, resources->branco);


            const char *labels[2] = { "Sair do jogo", "Voltar ao menu" };
            UiOpcaoEstilo estilo = {
                .size_normal  = 48.0f,
                .size_select  = 60.0f,
                .cor_normal   = resources->branco,
                .cor_select   = resources->amarelo,
                .espacamento  = 70.0f
            };
            UiOpcaoDraw(resources->fonte_principal, labels, 2,
                        self->opc_confirmacao, estilo, scream_height * 0.65f, scream_width);
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