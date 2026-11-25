/** @file cafeteira.c
 * @brief Implementação da cafeteira da copa.
 */
#include "scenes/cafeteira.h"
#include "map/map.h"
#include <math.h>

void CafeteiraInit(CafeteiraBody *self) {
    self->usada     = false;
    self->ativa     = false;
    self->tempo_fim = 0.0f;
    self->aviso_timer = 0.0f;

    // TODO: ajuste para a posição real da cafeteira (x1, y1, x2, y2 em pixels do mapa)
    const float x1 = 3050, y1 = 407, x2 = 3118, y2 = 480;
    self->objeto = InteragivelCriar(x1 * MAP_ESCALA, y1 * MAP_ESCALA,
                                    (x2 - x1) * MAP_ESCALA, (y2 - y1) * MAP_ESCALA, 40.0f);
}

void CafeteiraAtivar(CafeteiraBody *self, PlayerBody *player, float tempo_restante) {
    self->usada         = true;
    self->ativa         = true;
    self->objeto.ativo  = false;
    self->tempo_fim     = tempo_restante - CAFETEIRA_DURACAO;
    player->correndo    = true;
    self->aviso_timer   = CAFETEIRA_AVISO_DURACAO;
}

void CafeteiraUpdate(CafeteiraBody *self, PlayerBody *player, float tempo_restante) {
    if (self->aviso_timer > 0.0f) {
        self->aviso_timer -= GetFrameTime();
        if (self->aviso_timer < 0.0f) self->aviso_timer = 0.0f;
    }

    if (!self->ativa) return;

    if (tempo_restante <= self->tempo_fim) {
        self->ativa      = false;
        player->correndo = false;
    }
}

void CafeteiraDrawAviso(CafeteiraBody *self, ResourcesBody *resources,
                        int screen_width, int screen_height) {
    if (self->aviso_timer <= 0.0f) return;

    // Opacidade: 1.0 durante quase todo o tempo, cai até 0 no último segundo
    float alpha = (self->aviso_timer < CAFETEIRA_AVISO_FADE)
                  ? self->aviso_timer / CAFETEIRA_AVISO_FADE
                  : 1.0f;

    const char *linha1 = "VOCE BEBEU O CAFE E GANHOU ENERGIA!!";
    const char *linha2 = TextFormat("+%d MIN DE VELOCIDADE", (int)(CAFETEIRA_DURACAO / 60.0f));

    float tam1 = 56.0f, tam2 = 44.0f;
    Vector2 d1 = MeasureTextEx(resources->fonte_principal, linha1, tam1, 1);
    Vector2 d2 = MeasureTextEx(resources->fonte_principal, linha2, tam2, 1);

    float largura = fmaxf(d1.x, d2.x) + 80.0f;
    float altura  = d1.y + d2.y + 50.0f;
    float x = (screen_width - largura) / 2.0f;
    float y = screen_height * 0.18f;

    DrawRectangleRounded((Rectangle){ x, y, largura, altura }, 0.25f, 8,
                         Fade(BLACK, 0.75f * alpha));
    DrawRectangleRoundedLinesEx((Rectangle){ x, y, largura, altura }, 0.25f, 8, 3.0f,
                                Fade(resources->amarelo, alpha));

    DrawTextEx(resources->fonte_principal, linha1,
               (Vector2){ (screen_width - d1.x) / 2.0f, y + 20.0f },
               tam1, 1, Fade(resources->amarelo, alpha));
    DrawTextEx(resources->fonte_principal, linha2,
               (Vector2){ (screen_width - d2.x) / 2.0f, y + 20.0f + d1.y + 10.0f },
               tam2, 1, Fade(resources->ciano_claro, alpha));
}

void CafeteiraDrawIndicador(CafeteiraBody *self, ResourcesBody *resources, Camera2D camera) {
    if (!self->objeto.ativo) return;

    Vector2 pos_tela = GetWorldToScreen2D(
        (Vector2){ self->objeto.mundo_rect.x + self->objeto.mundo_rect.width / 2.0f,
                   self->objeto.mundo_rect.y - 40.0f },
        camera);

    const char *texto   = "E - tomar cafe";
    float       tamanho = 36.0f;
    Vector2     dim     = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);
    bool        visivel = ((int)(GetTime() * 2) % 2 == 0);
    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ pos_tela.x - dim.x / 2.0f, pos_tela.y },
               tamanho, 1, visivel ? resources->amarelo : resources->lilas_claro);
}

void CafeteiraDrawHUD(CafeteiraBody *self, ResourcesBody *resources,
                      float tempo_restante, int screen_width) {
    if (!self->ativa) return;

    int seg = (int)ceilf(tempo_restante - self->tempo_fim);
    if (seg < 0) seg = 0;

    const char *texto = TextFormat("CAFE %02d:%02d", seg / 60, seg % 60);
    Vector2 dim = MeasureTextEx(resources->fonte_principal, texto, 40.0f, 1);
    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ screen_width - dim.x - 30.0f, 100.0f },
               40.0f, 1, resources->marrom_cafe);
}