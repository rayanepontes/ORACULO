/** @file instrucoes.h
 * @brief Tela de instruções iniciais dos minigames (imagem + ENTER piscando).
 */
#ifndef INSTRUCOES_H
#define INSTRUCOES_H

#include <raylib.h>
#include <math.h>
#include "core/resources.h"

/** @brief Desenha a imagem de instruções ajustada à tela e o "ENTER" piscando no canto inferior direito. */
static inline void InstrucoesDraw(Texture2D tex, ResourcesBody *resources,
                                  int screen_width, int screen_height) {
    DrawRectangle(0, 0, screen_width, screen_height, BLACK);

    if (tex.id != 0) {
        float escala = fminf((float)screen_width  / tex.width,
                             (float)screen_height / tex.height);
        float dest_w = tex.width  * escala;
        float dest_h = tex.height * escala;
        DrawTexturePro(tex,
                       (Rectangle){ 0, 0, (float)tex.width, (float)tex.height },
                       (Rectangle){ (screen_width  - dest_w) / 2.0f,
                                    (screen_height - dest_h) / 2.0f,
                                    dest_w, dest_h },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    const char *texto = "ENTER";
    float tamanho = 50.0f;
    Vector2 dim = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);
    bool visivel = ((int)(GetTime() * 2) % 2 == 0);
    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ screen_width - dim.x - 40.0f, screen_height - dim.y - 30.0f },
               tamanho, 1, visivel ? resources->amarelo : resources->branco);
}

#endif