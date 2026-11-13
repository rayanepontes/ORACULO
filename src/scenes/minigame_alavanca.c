/** @file minigame_alavanca.c
 * @brief Implementação do minigame Alavanca de Emergência.
 */
#include "scenes/minigame_alavanca.h"
#include "map/map.h"
#include "graphics/instrucoes.h"
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
    self->instrucoes = LoadTexture(ALAVANCA_PATH "minigame02.png");
    self->mostrando_instrucoes = true;
    AlavancaResetarPainel(self);
    self->concluido = false;
    self->cancelado = false;

    self->alavanca_normal   = LoadTexture(ALAVANCA_PATH "alavanca_normal.png");
    self->alavanca_puxada   = LoadTexture(ALAVANCA_PATH "alavanca_puxada.png");
    self->botao_confirmar_n = LoadTexture(ALAVANCA_PATH "botao_confirmar_normal.png");
    self->botao_confirmar_s = LoadTexture(ALAVANCA_PATH "botao_confirnar_slected.png");
    self->botao_evacuar_n   = LoadTexture(ALAVANCA_PATH "botao_evacuar_normal.png");
    self->botao_evacuar_s   = LoadTexture(ALAVANCA_PATH "botao_evacuar_selected.png");

    // Coordenadas do objeto interagível no mapa (a ajustar conforme posição do painel)
    self->objeto = InteragivelCriar(3177 * MAP_ESCALA, 1757 * MAP_ESCALA,
                                    (3278 - 3177) * MAP_ESCALA, (1836 - 1757) * MAP_ESCALA, 40.0f);
}

void AlavancaUpdate(AlavancaBody *self, float *tempo_restante) {
    if (self->mostrando_instrucoes) {
        if (IsKeyPressed(KEY_ESCAPE)) self->cancelado = true;
        else if (IsKeyPressed(KEY_ENTER)) self->mostrando_instrucoes = false;
        return;
    }
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
        // Só permite digitar se o PIN ainda não foi confirmado
        if (!self->pin_confirmado) {
            // ── Digitação de dígitos ──────────────────────────────────────────
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
        }
        // ENTER confirma/desconfirma o PIN (não dispara animação)
        if (IsKeyPressed(KEY_ENTER)) {
            if (!self->pin_confirmado && self->cursor_digito == ALAVANCA_PIN_DIGITOS) {
                self->pin_confirmado = true;
            } else if (self->pin_confirmado) {
                // desconfirma para editar de novo
                self->pin_confirmado = false;
            }
        }
    } else {
        // ── Área EVACUAR: ENTER dispara a animação ────────────────────────────
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
    if (self->mostrando_instrucoes) {
        InstrucoesDraw(self->instrucoes, resources, screen_width, screen_height);
        return;
    }
    // ── Fundo: alavanca_normal ou alavanca_puxada ocupa a tela inteira ────────
    bool mostrar_puxada = self->anim_rodando ||
                          (self->tela_atual == TELA_ALAVANCA_ACERTO) ||
                          (self->tela_atual == TELA_ALAVANCA_ERRO);
    Texture2D fundo_tex = mostrar_puxada ? self->alavanca_puxada : self->alavanca_normal;

    if (fundo_tex.id != 0) {
        float escala = fminf((float)screen_width  / fundo_tex.width,
                             (float)screen_height / fundo_tex.height);
        float dest_w = fundo_tex.width  * escala;
        float dest_h = fundo_tex.height * escala;
        DrawTexturePro(fundo_tex,
                       (Rectangle){ 0, 0, (float)fundo_tex.width, (float)fundo_tex.height },
                       (Rectangle){ (screen_width  - dest_w) / 2.0f,
                                    (screen_height - dest_h) / 2.0f,
                                    dest_w, dest_h },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    // Coordenadas base proporcional à textura do fundo
    float escala_painel = 0.0f;
    float painel_x      = 0.0f;
    float painel_y      = 0.0f;
    if (fundo_tex.id != 0) {
        escala_painel = fminf((float)screen_width  / fundo_tex.width,
                              (float)screen_height / fundo_tex.height);
        painel_x = (screen_width  - fundo_tex.width  * escala_painel) / 2.0f;
        painel_y = (screen_height - fundo_tex.height * escala_painel) / 2.0f;
    }

    // ── Área do PIN ───────────────────────────────────────────────────────────
    // Coordenada de tela dos underlines (ponto superior-esquerdo na textura: 1093, 693)
    float campo_x  = painel_x + 1065.0f * escala_painel;
    float campo_cy = painel_y +  653.0f * escala_painel;

    // Exibe cada dígito (ou traço piscante se ainda não digitado e for o cursor)
    float dig_size = 60.0f;
    float dig_gap  = 50.0f;
    float dig_start_x = campo_x;

    bool cursor_piscando = ((int)(GetTime() * 2) % 2 == 0) &&
                           (self->tela_atual == TELA_ALAVANCA_PRINCIPAL) &&
                           (self->foco == FOCO_ALAVANCA_PIN) &&
                           !self->pin_confirmado &&
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

        DrawTextEx(resources->fonte_principal, digito_str,
                   (Vector2){ dig_start_x + i * (dig_size + dig_gap),
                               campo_cy },
                   dig_size, 1, cor_dig);
    }

    // ── Botão CONFIRMAR (selected quando foco=PIN e tela principal) ──────────
    bool confirmar_sel = (self->foco == FOCO_ALAVANCA_PIN) &&
                         (self->tela_atual == TELA_ALAVANCA_PRINCIPAL) &&
                         !self->anim_rodando;
    Texture2D botao_conf = confirmar_sel ? self->botao_confirmar_s : self->botao_confirmar_n;

    float conf_w = (float)botao_conf.width  * escala_painel;
    float conf_h = (float)botao_conf.height * escala_painel;
    float conf_x = painel_x + 951.0f * escala_painel;
    float conf_y = painel_y + 741.0f * escala_painel;
    if (botao_conf.id != 0) {
        DrawTexturePro(botao_conf,
                       (Rectangle){ 0, 0, (float)botao_conf.width, (float)botao_conf.height },
                       (Rectangle){ conf_x, conf_y, conf_w, conf_h },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    // ── Botão EVACUAR (selected quando foco=EVACUAR e tela principal) ─────────
    bool evacuar_sel = (self->foco == FOCO_ALAVANCA_EVACUAR) &&
                       (self->tela_atual == TELA_ALAVANCA_PRINCIPAL) &&
                       !self->anim_rodando;
    Texture2D botao_evac = evacuar_sel ? self->botao_evacuar_s : self->botao_evacuar_n;

    float evac_w = (float)botao_evac.width  * escala_painel;
    float evac_h = (float)botao_evac.height * escala_painel;
    float evac_x = painel_x + 926.0f * escala_painel;
    float evac_y = painel_y + 846.0f * escala_painel;
    if (botao_evac.id != 0) {
        DrawTexturePro(botao_evac,
                       (Rectangle){ 0, 0, (float)botao_evac.width, (float)botao_evac.height },
                       (Rectangle){ evac_x, evac_y, evac_w, evac_h },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    // ── Overlay de erro ───────────────────────────────────────────────────────
    if (self->tela_atual == TELA_ALAVANCA_ERRO) {
        DrawRectangle(0, 0, screen_width, screen_height, (Color){ 0, 0, 0, 210 });
        DrawRectangle(0, 0, screen_width, screen_height, (Color){ 180, 0, 0, 80 });

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
        DrawRectangle(0, 0, screen_width, screen_height, (Color){ 0, 0, 0, 220 });
        DrawRectangle(0, 0, screen_width, screen_height, (Color){ 0, 128, 0, 80 });

        float cx = screen_width / 2.0f;

        // Título
        const char *titulo = "PROTOCOLO DE EMERGENCIA CONCLUIDO";
        Vector2 dt = MeasureTextEx(resources->fonte_principal, titulo, 72.0f, 1);
        DrawTextEx(resources->fonte_principal, titulo,
                   (Vector2){ cx - dt.x / 2.0f, screen_height * 0.07f },
                   72.0f, 1, resources->amarelo);

        // Subtítulo
        const char *sub = "INTERVENCAO BEM-SUCEDIDA";
        Vector2 ds = MeasureTextEx(resources->fonte_principal, sub, 50.0f, 1);
        DrawTextEx(resources->fonte_principal, sub,
                   (Vector2){ cx - ds.x / 2.0f, screen_height * 0.18f },
                   50.0f, 1, resources->branco);

        // Bullets
        const char *bullets[] = {
            "* Falha do sensor identificada.",
            "* Informacao inconsistente desconsiderada.",
            "* Controle automatico interrompido.",
            "* Evacuacao autorizada por supervisao humana.",
        };
        float by = screen_height * 0.30f;
        for (int i = 0; i < 4; i++) {
            DrawTextEx(resources->fonte_principal, bullets[i],
                       (Vector2){ screen_width * 0.15f, by },
                       38.0f, 1, resources->branco);
            by += 52.0f;
        }

        // Linha da IA
        const char *ia_label = "* ORACULO_AI:";
        DrawTextEx(resources->fonte_principal, ia_label,
                   (Vector2){ screen_width * 0.15f, by },
                   38.0f, 1, resources->lilas_claro);
        by += 52.0f;

        const char *ia_msg = "\"Intervencao humana registrada. Resultado da operacao: satisfatorio.\"";
        Vector2 dim_ia = MeasureTextEx(resources->fonte_principal, ia_msg, 34.0f, 1);
        DrawTextEx(resources->fonte_principal, ia_msg,
                   (Vector2){ cx - dim_ia.x / 2.0f, by },
                   34.0f, 1, resources->lilas_claro);

        // ENTER para continuar (piscando)
        const char *continuar = "ENTER para continuar";
        Vector2 dc = MeasureTextEx(resources->fonte_principal, continuar, 50.0f, 1);
        bool v = ((int)(GetTime() * 2) % 2 == 0);
        DrawTextEx(resources->fonte_principal, continuar,
                   (Vector2){ cx - dc.x / 2.0f, screen_height * 0.90f },
                   50.0f, 1, v ? resources->amarelo : resources->branco);
    }
}

void AlavancaUnload(AlavancaBody *self) {
    UnloadTexture(self->instrucoes);
    UnloadTexture(self->alavanca_normal);
    UnloadTexture(self->alavanca_puxada);
    UnloadTexture(self->botao_confirmar_n);
    UnloadTexture(self->botao_confirmar_s);
    UnloadTexture(self->botao_evacuar_n);
    UnloadTexture(self->botao_evacuar_s);
}