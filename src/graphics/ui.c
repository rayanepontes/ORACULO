/**
 * @file ui.c
 * @brief Implementação dos componentes de interface de usuário reutilizáveis.
 */

#include "graphics/ui.h"

int UiOpcaoUpdate(int *opcao_atual, int total) {
    if (IsKeyPressed(KEY_DOWN)) {
        *opcao_atual = (*opcao_atual + 1) % total;
    }
    if (IsKeyPressed(KEY_UP)) {
        *opcao_atual = (*opcao_atual - 1 + total) % total;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        return *opcao_atual;
    }
    return -1;
}

void UiOpcaoDraw(Font fonte, const char **labels, int total, int opcao_atual,
                 UiOpcaoEstilo estilo, float pos_y_inicial, int scream_width) {

    for (int i = 0; i < total; i++) {
        bool  is_select = (i == opcao_atual);
        float size      = is_select ? estilo.size_select  : estilo.size_normal;
        Color cor       = is_select ? estilo.cor_select   : estilo.cor_normal;

        Vector2 dim = MeasureTextEx(fonte, labels[i], size, 1);
        Vector2 pos = {
            (scream_width / 2.0f) - (dim.x / 2.0f),
            pos_y_inicial + (i * estilo.espacamento)
        };

        DrawTextEx(fonte, labels[i], pos, size, 1, cor);
    }
}