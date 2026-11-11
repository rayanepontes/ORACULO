/**
 * @file alucinacao.c
 * @brief Implementação do minigame do Detector de Alucinação.
 * @details O jogador navega entre relatórios com ◄ ► e confirma a escolha
 *          com ENTER. Se errar, recebe uma mensagem de erro e tenta novamente.
 *          O minigame só fecha ao acertar ou ao pressionar ESC.
 */

#include <raylib.h>
#include "scenes/minigame_alucinacao.h"
#include "graphics/ui.h"
#include "map/map.h"
#include <math.h>

/** @brief Caminhos dos PNGs dos relatórios, na ordem dos índices. */
static const char *RELATORIO_CAMINHOS[ALUCINACAO_TOTAL_RELATORIOS] = {
    "assets/texture/MiniGame_Alucinacao/relatorio1.png",
    "assets/texture/MiniGame_Alucinacao/relatorio2.png",
    "assets/texture/MiniGame_Alucinacao/relatorio3.png"
};

// ─── Helpers de Update ────────────────────────────────────────────────────────

/** @brief Processa a navegação entre relatórios e abertura da confirmação.
 * @param self Ponteiro para o AlucinacaoBody ativo.
 */
static void AlucinacaoUpdateRelatorios(AlucinacaoBody *self) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        self->cancelado = true;
        return;
    }

    if (IsKeyPressed(KEY_RIGHT) &&
        self->relatorio_atual < ALUCINACAO_TOTAL_RELATORIOS - 1) {
        self->relatorio_atual++;
    }

    if (IsKeyPressed(KEY_LEFT) && self->relatorio_atual > 0) {
        self->relatorio_atual--;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        self->opc_confirmacao = ALUC_CONFIRMAR;
        self->tela_atual = TELA_ALUC_CONFIRMACAO;
    }
}

/** @brief Processa a confirmação ou cancelamento da escolha do relatório.
 * @param self Ponteiro para o AlucinacaoBody ativo.
 */
static void AlucinacaoUpdateConfirmacao(AlucinacaoBody *self) {
    int resultado = UiOpcaoUpdate((int *)&self->opc_confirmacao, ALUC_TOTAL_OPC);

    if (IsKeyPressed(KEY_ESCAPE) || resultado == ALUC_CANCELAR) {
        self->opc_confirmacao = ALUC_CONFIRMAR;
        self->tela_atual = TELA_ALUC_RELATORIOS;
        return;
    }

    if (resultado == ALUC_CONFIRMAR) {
        if (self->relatorio_atual == self->relatorio_correto) {
            self->concluido = true;
        } else {
            self->tela_atual = TELA_ALUC_ERRO;
        }
    }
}

/** @brief Processa a tela de erro — ENTER volta para a navegação.
 * @param self Ponteiro para o AlucinacaoBody ativo.
 */
static void AlucinacaoUpdateErro(AlucinacaoBody *self) {
    if (IsKeyPressed(KEY_ENTER)) {
        self->tela_atual = TELA_ALUC_RELATORIOS;
    }
}

// ─── Helpers de Draw ──────────────────────────────────────────────────────────

/** @brief Desenha o relatório atual centralizado e as setas de navegação.
 * @param self          Ponteiro para o AlucinacaoBody ativo.
 * @param resources     Recursos visuais (fonte, cores).
 * @param scream_width  Largura da tela em pixels.
 * @param scream_height Altura da tela em pixels.
 */
static void AlucinacaoDrawRelatorios(AlucinacaoBody *self, ResourcesBody *resources, int scream_width, int scream_height) {
    // Fundo
    DrawTexturePro(self->background,
        (Rectangle){ 0, 0, (float)self->background.width, (float)self->background.height },
        (Rectangle){ 0, 0, (float)scream_width, (float)scream_height },
        (Vector2){ 0, 0 }, 0.0f, WHITE);

    // Relatório centralizado
    Texture2D rel = self->relatorios[self->relatorio_atual];
    if (rel.id != 0) {
        float escala = fminf((scream_width  * 0.80f) / rel.width,
                             (scream_height * 0.80f) / rel.height);
        float dest_w = rel.width  * escala;
        float dest_h = rel.height * escala;
        Rectangle origem  = { 0.0f, 0.0f, (float)rel.width, (float)rel.height };
        Rectangle destino = { (scream_width  - dest_w) / 2.0f,
                              (scream_height - dest_h) / 2.0f,
                              dest_w, dest_h };
        DrawTexturePro(rel, origem, destino, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
    }

    // Seta esquerda (só se não for o primeiro)
    if (self->relatorio_atual > 0) {
        const char *seta = "<";
        Vector2 dim = MeasureTextEx(resources->fonte_principal, seta, 72.0f, 1);
        DrawTextEx(resources->fonte_principal, seta,
                   (Vector2){ 20.0f, (scream_height - dim.y) / 2.0f },
                   72.0f, 1, resources->amarelo);
    }

    // Seta direita (só se não for o último)
    if (self->relatorio_atual < ALUCINACAO_TOTAL_RELATORIOS - 1) {
        const char *seta = ">";
        Vector2 dim = MeasureTextEx(resources->fonte_principal, seta, 72.0f, 1);
        DrawTextEx(resources->fonte_principal, seta,
                   (Vector2){ scream_width - dim.x - 20.0f,
                               (scream_height - dim.y) / 2.0f },
                   72.0f, 1, resources->amarelo);
    }

    // Indicador de página (ex: 1 / 3)
    const char *indice = TextFormat("%d / %d", self->relatorio_atual + 1,
                                    ALUCINACAO_TOTAL_RELATORIOS);
    Vector2 ind_dim = MeasureTextEx(resources->fonte_principal, indice, 40.0f, 1);
    DrawTextEx(resources->fonte_principal, indice,
               (Vector2){ (scream_width - ind_dim.x) / 2.0f,
                           scream_height * 0.92f },
               40.0f, 1, resources->branco);

    // Instrução
    const char *instrucao = "ENTER - selecionar   ESC - sair";
    Vector2 inst_dim = MeasureTextEx(resources->fonte_principal, instrucao, 32.0f, 1);
    DrawTextEx(resources->fonte_principal, instrucao,
               (Vector2){ (scream_width - inst_dim.x) / 2.0f, scream_height * 0.96f },
               32.0f, 1, resources->lilas_claro);
}

/** @brief Desenha o overlay de confirmação de escolha do relatório.
 * @param self          Ponteiro para o AlucinacaoBody ativo.
 * @param resources     Recursos visuais (fonte, cores).
 * @param scream_width  Largura da tela em pixels.
 * @param scream_height Altura da tela em pixels.
 */
static void AlucinacaoDrawConfirmacao(AlucinacaoBody *self, ResourcesBody *resources, int scream_width, int scream_height) {
    // Fundo
    DrawTexturePro(self->background,
        (Rectangle){ 0, 0, (float)self->background.width, (float)self->background.height },
        (Rectangle){ 0, 0, (float)scream_width, (float)scream_height },
        (Vector2){ 0, 0 }, 0.0f, WHITE);

    const char *msg = TextFormat("Enviar o relatorio %d?", self->relatorio_atual + 1);
    Vector2 msg_dim = MeasureTextEx(resources->fonte_principal, msg, 80.0f, 1);
    DrawTextEx(resources->fonte_principal, msg,
               (Vector2){ (scream_width - msg_dim.x) / 2.0f, scream_height * 0.35f },
               80.0f, 1, resources->branco);

    const char *labels[ALUC_TOTAL_OPC] = { "CONFIRMAR", "VOLTAR" };
    UiOpcaoEstilo estilo = {
        .size_normal  = 60.0f,
        .size_select  = 72.0f,
        .cor_normal   = resources->branco,
        .cor_select   = resources->amarelo,
        .espacamento  = 90.0f
    };
    UiOpcaoDraw(resources->fonte_principal, (const char **)labels, ALUC_TOTAL_OPC,
                (int)self->opc_confirmacao, estilo, scream_height * 0.55f, scream_width);
}

/** @brief Desenha a mensagem de erro ao escolher o relatório errado.
 * @param resources     Recursos visuais (fonte, cores).
 * @param scream_width  Largura da tela em pixels.
 * @param scream_height Altura da tela em pixels.
 */
static void AlucinacaoDrawErro(ResourcesBody *resources,
                                int scream_width, int scream_height) {
    DrawRectangle(0, 0, scream_width, scream_height, (Color){ 0, 0, 0, 220 });

    const char *msg = "Relatorio incorreto!";
    Vector2 msg_dim = MeasureTextEx(resources->fonte_principal, msg, 80.0f, 1);
    DrawTextEx(resources->fonte_principal, msg,
               (Vector2){ (scream_width - msg_dim.x) / 2.0f, scream_height * 0.35f },
               80.0f, 1, RED);

    const char *sub = "Revise o manual e tente novamente.";
    Vector2 sub_dim = MeasureTextEx(resources->fonte_principal, sub, 48.0f, 1);
    DrawTextEx(resources->fonte_principal, sub,
               (Vector2){ (scream_width - sub_dim.x) / 2.0f, scream_height * 0.50f },
               48.0f, 1, resources->branco);

    const char *instrucao = "ENTER - tentar novamente";
    Vector2 inst_dim = MeasureTextEx(resources->fonte_principal, instrucao, 36.0f, 1);
    DrawTextEx(resources->fonte_principal, instrucao,
               (Vector2){ (scream_width - inst_dim.x) / 2.0f, scream_height * 0.70f },
               36.0f, 1, resources->amarelo);
}

// ─── API pública ──────────────────────────────────────────────────────────────

void AlucinacaoInit(AlucinacaoBody *self) {
    self->background = LoadTexture("assets/texture/MiniGame_Alucinacao/Detector de Alucinacao BG.png");
    self->tela_atual        = TELA_ALUC_RELATORIOS;
    self->relatorio_atual   = 0;
    self->relatorio_correto = ALUCINACAO_RELATORIO_CORRETO;
    self->opc_confirmacao   = ALUC_CONFIRMAR;
    self->concluido         = false;
    self->cancelado         = false;

    for (int i = 0; i < ALUCINACAO_TOTAL_RELATORIOS; i++) {
        self->relatorios[i] = LoadTexture(RELATORIO_CAMINHOS[i]);
    }

    self->computador_sprite = LoadTexture("assets/texture/MiniGame_Alucinacao/Computador alucinacao.png");

    float comp_w = self->computador_sprite.width  / (MAP_ESCALA * 1.5f);
    float comp_h = self->computador_sprite.height / (MAP_ESCALA * 1.5f);
    float comp_x = 1260 * MAP_ESCALA;
    float comp_y = 200  * MAP_ESCALA;
    self->computador = InteragivelCriar(comp_x, comp_y, comp_w, comp_h, 40.0f);
}

void AlucinacaoUpdate(AlucinacaoBody *self) {
    switch (self->tela_atual) {
        case TELA_ALUC_RELATORIOS:
            AlucinacaoUpdateRelatorios(self);
            break;
        case TELA_ALUC_CONFIRMACAO:
            AlucinacaoUpdateConfirmacao(self);
            break;
        case TELA_ALUC_ERRO:
            AlucinacaoUpdateErro(self);
            break;
        default:
            break;
    }
}

/** @brief Desenha o indicador "E - interagir" acima do computador quando o player está próximo.
 * @param self          Ponteiro para o AlucinacaoBody ativo.
 * @param resources     Recursos visuais (fonte, cores).
 * @param camera        Câmera 2D atual (para converter posição do mundo para tela).
 * @param scream_width  Largura da tela em pixels.
 */
void AlucinacaoDrawIndicador(AlucinacaoBody *self, ResourcesBody *resources, Camera2D camera) {
    // Centro superior do computador em coordenadas de tela
    Vector2 pos_mundo = {
        self->computador.mundo_rect.x + self->computador.mundo_rect.width / 2.0f,
        self->computador.mundo_rect.y - 40.0f
    };
    Vector2 pos_tela = GetWorldToScreen2D(pos_mundo, camera);

    const char *texto = "E - interagir";
    float tamanho = 36.0f;
    Vector2 dim = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);

    // Pisca 2x por segundo
    bool visivel = ((int)(GetTime() * 2) % 2 == 0);
    Color cor = visivel ? resources->amarelo : resources->lilas_claro;

    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ pos_tela.x - dim.x / 2.0f, pos_tela.y },
               tamanho, 1, cor);
}

void AlucinacaoDrawMundo(AlucinacaoBody *self) {
    DrawTexturePro(
        self->computador_sprite,
        (Rectangle){ 0, 0, (float)self->computador_sprite.width,
                           (float)self->computador_sprite.height },
        self->computador.mundo_rect,
        (Vector2){ 0, 0 }, 0.0f, WHITE
    );
}

void AlucinacaoDraw(AlucinacaoBody *self, ResourcesBody *resources,
                    int scream_width, int scream_height) {
    switch (self->tela_atual) {
        case TELA_ALUC_RELATORIOS:
            AlucinacaoDrawRelatorios(self, resources, scream_width, scream_height);
            break;
        case TELA_ALUC_CONFIRMACAO:
            AlucinacaoDrawConfirmacao(self, resources, scream_width, scream_height);
            break;
        case TELA_ALUC_ERRO:
            AlucinacaoDrawErro(resources, scream_width, scream_height);
            break;
        default:
            break;
    }
}

void AlucinacaoUnload(AlucinacaoBody *self) {
    UnloadTexture(self->computador_sprite);
    UnloadTexture(self->background);
    for (int i = 0; i < ALUCINACAO_TOTAL_RELATORIOS; i++) {
        UnloadTexture(self->relatorios[i]);
    }
}