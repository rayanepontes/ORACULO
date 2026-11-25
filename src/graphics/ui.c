/**
 * @file ui.c
 * @brief Implementação dos componentes de interface de usuário reutilizáveis.
 */

#include "graphics/ui.h"

void UiOpcaoCalcRects(Font fonte, const char **labels, int total,
                      UiOpcaoEstilo estilo, float pos_y_inicial,
                      int scream_width, Rectangle *rects_out) {
    for (int i = 0; i < total; i++) {
        // usa size_select para o rect ser grande o suficiente em qualquer estado
        Vector2 dim = MeasureTextEx(fonte, labels[i], estilo.size_select, 1);
        rects_out[i] = (Rectangle){
            (scream_width / 2.0f) - (dim.x / 2.0f),
            pos_y_inicial + (i * estilo.espacamento),
            dim.x,
            dim.y
        };
    }
}

int UiOpcaoUpdate(int *opcao_atual, int total, Sound *som, Sound *som_confirm, Rectangle *rects) {
    bool mudou = false;

    // ── Teclado / WASD ────────────────────────────────────────────────────────
    if (InputBaixo()) {
        *opcao_atual = (*opcao_atual + 1) % total;
        mudou = true;
    }
    if (InputCima()) {
        *opcao_atual = (*opcao_atual - 1 + total) % total;
        mudou = true;
    }
    if (mudou && som) PlaySound(*som);
    if (IsKeyPressed(KEY_ENTER)) {
        if (som_confirm) PlaySound(*som_confirm);
        return *opcao_atual;
    }

    // ── Mouse ─────────────────────────────────────────────────────────────────
    if (rects) {
        Vector2 mouse = GetMousePosition();
        for (int i = 0; i < total; i++) {
            if (CheckCollisionPointRec(mouse, rects[i])) {
                // hover: destaca a opção e toca o som
                    if (*opcao_atual != i) {
                        *opcao_atual = i;
                        if (som) PlaySound(*som);
                    }
                // clique: confirma
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (som_confirm) PlaySound(*som_confirm);
                    return i;
                }
            }
        }
    }

    return -1;
}

void UiOpcaoDraw(Font fonte, const char **labels, int total, int opcao_atual,
                 UiOpcaoEstilo estilo, float pos_y_inicial, int scream_width) {

    for (int i = 0; i < total; i++) {
        bool  is_select = (i == opcao_atual);
        float size      = is_select ? estilo.size_select : estilo.size_normal;
        Color cor       = is_select ? estilo.cor_select  : estilo.cor_normal;

        Vector2 dim = MeasureTextEx(fonte, labels[i], size, 1);
        Vector2 pos = {
            (scream_width / 2.0f) - (dim.x / 2.0f),
            pos_y_inicial + (i * estilo.espacamento)
        };

        DrawTextEx(fonte, labels[i], pos, size, 1, cor);
    }
}