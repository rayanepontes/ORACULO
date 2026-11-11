/**
 * @file interagivel.c
 * @brief Implementação do utilitário de objetos interativos.
 */

#include "core/interagivel.h"

Interagivel InteragivelCriar(float x, float y, float w, float h, float margem) {
    Interagivel obj;
    obj.mundo_rect    = (Rectangle){ x, y, w, h };
    obj.interacao_rect = (Rectangle){
        x - margem,
        y - margem,
        w + margem * 2.0f,
        h + margem * 2.0f
    };
    obj.ativo = true;
    return obj;
}

bool InteragivelPoderInteragir(Interagivel *self, Vector2 ponto) {
    if (!self->ativo) return false;
    return CheckCollisionPointRec(ponto, self->interacao_rect);
}