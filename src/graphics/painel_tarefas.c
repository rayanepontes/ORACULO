/**
 * @file painel_tarefas.c
 * @brief Implementação do painel deslizante de tarefas.
 */

#include "graphics/painel_tarefas.h"
#include <math.h>

/** @brief Caminhos das imagens (ajuste para onde você salvar os arquivos). */
#define PAINEL_CAMINHO_PAINEL "assets/texture/tarefas/tasks.png"
#define PAINEL_CAMINHO_CHECK  "assets/texture/tarefas/check.png"

/** @brief Mude para 1 para desenhar o contorno da aba (ajuda a calibrar). */
#define PAINEL_DEBUG_ABA 0

// ─── Helpers internos ─────────────────────────────────────────────────────────

/** @brief Retângulo do painel na tela, de acordo com a animação atual. */
static Rectangle PainelTarefasRetangulo(const PainelTarefasBody *self, float escala) {
    // Easing cúbico: começa rápido e desacelera ao chegar
    float inv = 1.0f - self->progresso;
    float t   = 1.0f - inv * inv * inv;

    float largura   = self->painel.width  * escala;
    float altura    = self->painel.height * escala;
    float x_fechado = (PAINEL_TAB_LARGURA * escala) - largura;  // só a aba aparece

    return (Rectangle){ x_fechado * (1.0f - t), PAINEL_POS_Y_REF * escala, largura, altura };
}

/** @brief Retângulo clicável da aba, dentro do retângulo do painel. */
static Rectangle PainelTarefasAba(Rectangle painel, float escala) {
    return (Rectangle){
        painel.x + painel.width - PAINEL_TAB_LARGURA * escala,
        painel.y + PAINEL_TAB_Y * escala,
        PAINEL_TAB_LARGURA * escala,
        PAINEL_TAB_ALTURA  * escala
    };
}

// ─── Funções públicas ─────────────────────────────────────────────────────────

void PainelTarefasInit(PainelTarefasBody *self) {
    self->painel    = LoadTexture(PAINEL_CAMINHO_PAINEL);
    self->check     = LoadTexture(PAINEL_CAMINHO_CHECK);
    self->progresso = 0.0f;
    self->aberto    = false;

    if (self->painel.id == 0) TraceLog(LOG_WARNING, "PAINEL: falha ao carregar %s", PAINEL_CAMINHO_PAINEL);
    if (self->check.id  == 0) TraceLog(LOG_WARNING, "PAINEL: falha ao carregar %s", PAINEL_CAMINHO_CHECK);
}

void PainelTarefasUpdate(PainelTarefasBody *self, int scream_height, Sound *som) {
    if (self->painel.id == 0) return;

    float escala = scream_height / PAINEL_ALTURA_REFERENCIA;

    // ── Input: TAB ou clique na aba ───────────────────────────────────────────
    bool alternar = IsKeyPressed(KEY_TAB);

    if (!alternar && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Rectangle painel = PainelTarefasRetangulo(self, escala);
        Rectangle aba    = PainelTarefasAba(painel, escala);
        alternar = CheckCollisionPointRec(GetMousePosition(), aba);
    }

    if (alternar) {
        self->aberto = !self->aberto;
        if (som) PlaySound(*som);
    }

    // ── Animação de deslize ───────────────────────────────────────────────────
    float alvo  = self->aberto ? 1.0f : 0.0f;
    float passo = GetFrameTime() / PAINEL_DURACAO_SLIDE;

    if (self->progresso < alvo)      self->progresso = fminf(alvo, self->progresso + passo);
    else if (self->progresso > alvo) self->progresso = fmaxf(alvo, self->progresso - passo);
}

void PainelTarefasDraw(PainelTarefasBody *self, const bool *concluidas, int scream_height) {
    if (self->painel.id == 0) return;

    float escala = scream_height / PAINEL_ALTURA_REFERENCIA;
    Rectangle destino = PainelTarefasRetangulo(self, escala);
    Rectangle origem  = { 0.0f, 0.0f, (float)self->painel.width, (float)self->painel.height };

    DrawTexturePro(self->painel, origem, destino, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

    // ── Checks das tarefas concluídas ─────────────────────────────────────────
    if (self->check.id != 0) {
        Rectangle origem_check = { 0.0f, 0.0f, (float)self->check.width, (float)self->check.height };
        float w = self->check.width  * escala;
        float h = self->check.height * escala;

        for (int i = 0; i < PAINEL_TOTAL_TAREFAS; i++) {
            if (!concluidas[i]) continue;

            float centro_x = destino.x + PAINEL_CHECK_X * escala;
            float centro_y = destino.y + (PAINEL_LINHA_Y0 + i * PAINEL_LINHA_PASSO) * escala;
            Rectangle dest_check = { centro_x - w / 2.0f, centro_y - h / 2.0f, w, h };

            DrawTexturePro(self->check, origem_check, dest_check, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        }
    }

#if PAINEL_DEBUG_ABA
    DrawRectangleLinesEx(PainelTarefasAba(destino, escala), 2, GREEN);
#endif
}

void PainelTarefasUnload(PainelTarefasBody *self) {
    UnloadTexture(self->painel);
    UnloadTexture(self->check);
}