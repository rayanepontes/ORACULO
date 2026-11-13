/** @file minigame_alavanca.c
 * @brief Implementação do minigame Alavanca de Emergência.
 */
#include "scenes/minigame_alavanca.h"
#include "map/map.h"
#include <math.h>
#include <stdio.h>

// ─── Assets ───────────────────────────────────────────────────────────────────

#define ALAVANCA_PATH "assets/texture/minigame_alavanca/"

// ─── Helpers internos ─────────────────────────────────────────────────────────

/** @brief Retorna o valor numérico do PIN digitado (ex: dígitos {6,4} → 64). */
static int AlavancaGetPin(AlavancaBody *self) {
    return self->digitos[0] * 10 + self->digitos[1];
}

/** @brief Reseta apenas os campos do painel sem recarregar assets. */
static void AlavancaResetarPainel(AlavancaBody *self) {
    self->tela_atual      = TELA_ALAVANCA_PRINCIPAL;
    self->foco            = FOCO_ALAVANCA_PIN;
    self->digitos[0]      = 0;
    self->digitos[1]      = 0;
    self->cursor_digito   = 0;
    self->pin_confirmado  = false;
    self->anim_timer      = 0.0f;
    self->anim_rodando    = false;
    self->anim_acertou    = false;
}

// ─── API pública ──────────────────────────────────────────────────────────────

void AlavancaInit(AlavancaBody *self) {
    AlavancaResetarPainel(self);
    self->concluido = false;
    self->cancelado = false;

    self->painel_design     = LoadTexture(ALAVANCA_PATH "alavanca_design.png");
    self->alavanca_normal   = LoadTexture(ALAVANCA_PATH "alavanca_normal.png");
    self->alavanca_puxada   = LoadTexture(ALAVANCA_PATH "alavanca_puxada.png");
    self->botao_confirmar_n = LoadTexture(ALAVANCA_PATH "botao_confirmar_normal.png");
    self->botao_confirmar_s = LoadTexture(ALAVANCA_PATH "botao_confirnar_slected.png");
    self->botao_evacuar_n   = LoadTexture(ALAVANCA_PATH "botao_evacuar_normal.png");
    self->botao_evacuar_s   = LoadTexture(ALAVANCA_PATH "botao_evacuar_selected.png");

    // Coordenadas do objeto interagível no mapa (a ajustar conforme posição do painel)
    self->objeto = InteragivelCriar(3177 * MAP_ESCALA, 1757 * MAP_ESCALA, 3278* MAP_ESCALA, 1836 * MAP_ESCALA, 40.0f);
}

void AlavancaUpdate(AlavancaBody *self, float *tempo_restante) {
    // ── Animação de descida da alavanca ──────────────────────────────────────
    if (self->anim_rodando) {
        self->anim_timer += GetFrameTime();
        if (self->anim_timer >= ALAVANCA_ANIM_TEMPO) {
            self->anim_rodando = false;
            if (self->anim_acertou) {
                self->tela_atual = TELA_ALAVANCA_ACERTO;
            } else {
                // Penalidade e volta ao estado inicial
                *tempo_restante -= ALAVANCA_PENALIDADE;
                if (*tempo_restante < 0.0f) *tempo_restante = 0.0f;
                self->tela_atual = TELA_ALAVANCA_ERRO;
            }
        }
        return;
    }

    // ── Tela de erro: aguarda ENTER pra tentar de novo ────────────────────────
    if (self->tela_atual == TELA_ALAVANCA_ERRO) {
        if (IsKeyPressed(KEY_ENTER)) {
            AlavancaResetarPainel(self);
        }
        return;
    }

    // ── Tela de acerto: aguarda ENTER ────────────────────────────────────────
    if (self->tela_atual == TELA_ALAVANCA_ACERTO) {
        if (IsKeyPressed(KEY_ENTER)) {
            self->concluido = true;
        }
        return;
    }

    // ── Tela principal ────────────────────────────────────────────────────────
    if (IsKeyPressed(KEY_ESCAPE)) {
        self->cancelado = true;
        return;
    }

    // Navegar entre áreas com setas verticais
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
        self->foco = (self->foco == FOCO_ALAVANCA_PIN)
                     ? FOCO_ALAVANCA_EVACUAR
                     : FOCO_ALAVANCA_PIN;
    }

    if (self->foco == FOCO_ALAVANCA_PIN) {
        // ── Digitação de dígitos ──────────────────────────────────────────────
        for (int k = KEY_ZERO; k <= KEY_NINE; k++) {
            if (IsKeyPressed(k) && self->cursor_digito < ALAVANCA_PIN_DIGITOS) {
                self->digitos[self->cursor_digito] = k - KEY_ZERO;
                self->cursor_digito++;
                break;
            }
        }
        // Teclas numéricas do numpad
        for (int k = KEY_KP_0; k <= KEY_KP_9; k++) {
            if (IsKeyPressed(k) && self->cursor_digito < ALAVANCA_PIN_DIGITOS) {
                self->digitos[self->cursor_digito] = k - KEY_KP_0;
                self->cursor_digito++;
                break;
            }
        }
        // Backspace apaga o último dígito
        if (IsKeyPressed(KEY_BACKSPACE) && self->cursor_digito > 0) {
            self->cursor_digito--;
            self->digitos[self->cursor_digito] = 0;
        }
        // ENTER confirma o PIN
        if (IsKeyPressed(KEY_ENTER)) {
            self->anim_rodando = true;
            self->anim_timer   = 0.0f;
            self->anim_acertou = (AlavancaGetPin(self) == ALAVANCA_PIN_CORRETO);
        }
    } else {
        // ── Área EVACUAR ──────────────────────────────────────────────────────
        if (IsKeyPressed(KEY_ENTER)) {
            self->anim_rodando = true;
            self->anim_timer   = 0.0f;
            self->anim_acertou = (AlavancaGetPin(self) == ALAVANCA_PIN_CORRETO);
        }
    }
}

void AlavancaDrawIndicador(AlavancaBody *self, ResourcesBody *resources, Camera2D camera) {
    if (!self->objeto.ativo) return;

    Vector2 pos_tela = GetWorldToScreen2D(
        (Vector2){ self->objeto.mundo_rect.x + self->objeto.mundo_rect.width  / 2.0f,
                   self->objeto.mundo_rect.y - 40.0f },
        camera);

    const char *texto   = "E - interagir";
    float       tamanho = 36.0f;
    Vector2     dim     = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);
    bool        visivel = ((int)(GetTime() * 2) % 2 == 0);
    Color       cor     = visivel ? resources->amarelo : resources->lilas_claro;
    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ pos_tela.x - dim.x / 2.0f, pos_tela.y },
               tamanho, 1, cor);
}

void AlavancaDraw(AlavancaBody *self, ResourcesBody *resources,
                  int screen_width, int screen_height) {

    // ── Fundo do painel ───────────────────────────────────────────────────────
    if (self->painel_design.id != 0) {
        float escala = fminf((float)screen_width  / self->painel_design.width,
                             (float)screen_height / self->painel_design.height);
        float dest_w = self->painel_design.width  * escala;
        float dest_h = self->painel_design.height * escala;
        DrawTexturePro(self->painel_design,
                       (Rectangle){ 0, 0, (float)self->painel_design.width,
                                          (float)self->painel_design.height },
                       (Rectangle){ (screen_width  - dest_w) / 2.0f,
                                    (screen_height - dest_h) / 2.0f,
                                    dest_w, dest_h },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    // Coordenadas base do painel centralizado (usadas para posicionar os elementos)
    float escala_painel = 0.0f;
    float painel_x      = 0.0f;
    float painel_y      = 0.0f;
    if (self->painel_design.id != 0) {
        escala_painel = fminf((float)screen_width  / self->painel_design.width,
                              (float)screen_height / self->painel_design.height);
        painel_x = (screen_width  - self->painel_design.width  * escala_painel) / 2.0f;
        painel_y = (screen_height - self->painel_design.height * escala_painel) / 2.0f;
    }

    // ── Alavanca (normal ou puxada) ───────────────────────────────────────────
    bool mostrar_puxada = self->anim_rodando ||
                         (self->tela_atual == TELA_ALAVANCA_ACERTO) ||
                         (self->tela_atual == TELA_ALAVANCA_ERRO && self->anim_acertou == false);
    Texture2D alavanca_tex = mostrar_puxada ? self->alavanca_puxada : self->alavanca_normal;

    // Posição da alavanca: lado direito do painel (proporcional)
    float alav_x = painel_x + self->painel_design.width * escala_painel * 0.72f;
    float alav_y = painel_y + self->painel_design.height * escala_painel * 0.10f;
    float alav_w = self->painel_design.width  * escala_painel * 0.28f;
    float alav_h = self->painel_design.height * escala_painel * 0.80f;
    if (alavanca_tex.id != 0) {
        DrawTexturePro(alavanca_tex,
                       (Rectangle){ 0, 0, (float)alavanca_tex.width, (float)alavanca_tex.height },
                       (Rectangle){ alav_x, alav_y, alav_w, alav_h },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    // ── Área do PIN ───────────────────────────────────────────────────────────
    // Campo de dígitos
    float campo_cx = painel_x + self->painel_design.width * escala_painel * 0.37f;
    float campo_cy = painel_y + self->painel_design.height * escala_painel * 0.56f;

    // Exibe cada dígito (ou traço piscante se ainda não digitado e for o cursor)
    float dig_size = 60.0f;
    float dig_gap  = 50.0f;
    float dig_total_w = ALAVANCA_PIN_DIGITOS * dig_size + (ALAVANCA_PIN_DIGITOS - 1) * dig_gap;
    float dig_start_x = campo_cx - dig_total_w / 2.0f;

    bool cursor_piscando = ((int)(GetTime() * 2) % 2 == 0) &&
                           (self->tela_atual == TELA_ALAVANCA_PRINCIPAL) &&
                           (self->foco == FOCO_ALAVANCA_PIN) &&
                           !self->anim_rodando;

    for (int i = 0; i < ALAVANCA_PIN_DIGITOS; i++) {
        const char *digito_str;
        Color cor_dig;

        if (i < self->cursor_digito) {
            // Dígito já inserido
            digito_str = TextFormat("%d", self->digitos[i]);
            cor_dig    = resources->branco;
        } else if (i == self->cursor_digito && cursor_piscando) {
            // Cursor piscante
            digito_str = "_";
            cor_dig    = resources->amarelo;
        } else {
            digito_str = "_";
            cor_dig    = (Color){ 100, 100, 100, 255 };
        }

        Vector2 dim = MeasureTextEx(resources->fonte_principal, digito_str, dig_size, 1);
        DrawTextEx(resources->fonte_principal, digito_str,
                   (Vector2){ dig_start_x + i * (dig_size + dig_gap) + (dig_size - dim.x) / 2.0f,
                               campo_cy - dim.y / 2.0f },
                   dig_size, 1, cor_dig);
    }

    // ── Botão CONFIRMAR ───────────────────────────────────────────────────────
    bool confirmar_sel = (self->foco == FOCO_ALAVANCA_PIN) &&
                         (self->tela_atual == TELA_ALAVANCA_PRINCIPAL) &&
                         !self->anim_rodando;
    Texture2D botao_conf = confirmar_sel ? self->botao_confirmar_s : self->botao_confirmar_n;

    float conf_w = self->painel_design.width  * escala_painel * 0.26f;
    float conf_h = self->painel_design.height * escala_painel * 0.12f;
    float conf_x = campo_cx - conf_w / 2.0f;
    float conf_y = painel_y + self->painel_design.height * escala_painel * 0.68f;
    if (botao_conf.id != 0) {
        DrawTexturePro(botao_conf,
                       (Rectangle){ 0, 0, (float)botao_conf.width, (float)botao_conf.height },
                       (Rectangle){ conf_x, conf_y, conf_w, conf_h },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    // ── Botão EVACUAR ─────────────────────────────────────────────────────────
    bool evacuar_sel = (self->foco == FOCO_ALAVANCA_EVACUAR) &&
                       (self->tela_atual == TELA_ALAVANCA_PRINCIPAL) &&
                       !self->anim_rodando;
    Texture2D botao_evac = evacuar_sel ? self->botao_evacuar_s : self->botao_evacuar_n;

    float evac_w = self->painel_design.width  * escala_painel * 0.26f;
    float evac_h = self->painel_design.height * escala_painel * 0.12f;
    float evac_x = campo_cx - evac_w / 2.0f;
    float evac_y = painel_y + self->painel_design.height * escala_painel * 0.82f;
    if (botao_evac.id != 0) {
        DrawTexturePro(botao_evac,
                       (Rectangle){ 0, 0, (float)botao_evac.width, (float)botao_evac.height },
                       (Rectangle){ evac_x, evac_y, evac_w, evac_h },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    // ── Overlay de erro ───────────────────────────────────────────────────────
    if (self->tela_atual == TELA_ALAVANCA_ERRO) {
        DrawRectangle(0, 0, screen_width, screen_height, (Color){ 180, 0, 0, 120 });

        const char *msg1 = "PIN INCORRETO";
        const char *msg2 = TextFormat("-%d SEGUNDOS", (int)ALAVANCA_PENALIDADE);
        const char *msg3 = "ENTER para tentar novamente";

        Vector2 d1 = MeasureTextEx(resources->fonte_principal, msg1, 90.0f, 1);
        Vector2 d2 = MeasureTextEx(resources->fonte_principal, msg2, 72.0f, 1);
        Vector2 d3 = MeasureTextEx(resources->fonte_principal, msg3, 50.0f, 1);

        DrawTextEx(resources->fonte_principal, msg1,
                   (Vector2){ screen_width / 2.0f - d1.x / 2.0f, screen_height * 0.35f },
                   90.0f, 1, RED);
        DrawTextEx(resources->fonte_principal, msg2,
                   (Vector2){ screen_width / 2.0f - d2.x / 2.0f, screen_height * 0.52f },
                   72.0f, 1, resources->amarelo);

        bool v = ((int)(GetTime() * 2) % 2 == 0);
        DrawTextEx(resources->fonte_principal, msg3,
                   (Vector2){ screen_width / 2.0f - d3.x / 2.0f, screen_height * 0.72f },
                   50.0f, 1, v ? resources->branco : resources->amarelo);
        return;
    }

    // ── Overlay de acerto ─────────────────────────────────────────────────────
    if (self->tela_atual == TELA_ALAVANCA_ACERTO) {
        DrawRectangle(0, 0, screen_width, screen_height, (Color){ 0, 0, 0, 160 });

        const char *msg1 = "EVACUACAO INICIADA";
        const char *msg2 = "ENTER para continuar";

        Vector2 d1 = MeasureTextEx(resources->fonte_principal, msg1, 90.0f, 1);
        Vector2 d2 = MeasureTextEx(resources->fonte_principal, msg2, 50.0f, 1);

        DrawTextEx(resources->fonte_principal, msg1,
                   (Vector2){ screen_width / 2.0f - d1.x / 2.0f, screen_height * 0.38f },
                   90.0f, 1, resources->amarelo);

        bool v = ((int)(GetTime() * 2) % 2 == 0);
        DrawTextEx(resources->fonte_principal, msg2,
                   (Vector2){ screen_width / 2.0f - d2.x / 2.0f, screen_height * 0.72f },
                   50.0f, 1, v ? resources->branco : resources->amarelo);
    }
}

void AlavancaUnload(AlavancaBody *self) {
    UnloadTexture(self->painel_design);
    UnloadTexture(self->alavanca_normal);
    UnloadTexture(self->alavanca_puxada);
    UnloadTexture(self->botao_confirmar_n);
    UnloadTexture(self->botao_confirmar_s);
    UnloadTexture(self->botao_evacuar_n);
    UnloadTexture(self->botao_evacuar_s);
}