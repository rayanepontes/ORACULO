/** @file minigame_triador.c
 * @brief Implementação do minigame Triador de Treinamento.
 */
#include "scenes/minigame_triador.h"
#include "graphics/ui.h"
#include "map/map.h"
#include "graphics/instrucoes.h"
#include <math.h>

// ─── Assets ───────────────────────────────────────────────────────────────────

#define TRIADOR_PATH        "assets/texture/minigame_triador/"
#define TRIADOR_COLAB_PATH  TRIADOR_PATH "colaboradores/"

static const char *AVATAR_CAMINHOS[TRIADOR_TOTAL_COLABORADORES] = {
    TRIADOR_COLAB_PATH "elena_voss.png",
    TRIADOR_COLAB_PATH "priya_nandan.png",
    TRIADOR_COLAB_PATH "gabriel_almeida.png",
    TRIADOR_COLAB_PATH "marco_aline.png",
    TRIADOR_COLAB_PATH "olivia_duarte.png",
    TRIADOR_COLAB_PATH "teo_vasques.png",
    TRIADOR_COLAB_PATH "ana_reis.png",
    TRIADOR_COLAB_PATH "augusto_ferraz.png",
};

static const char *AVATAR_3X4_CAMINHOS[TRIADOR_TOTAL_COLABORADORES] = {
    TRIADOR_COLAB_PATH "elena_3x4.png",
    TRIADOR_COLAB_PATH "priya_3x4.png",
    TRIADOR_COLAB_PATH "gabriel_3x4.png",
    TRIADOR_COLAB_PATH "marco_3x4.png",
    TRIADOR_COLAB_PATH "olivia_3x4.png",
    TRIADOR_COLAB_PATH "teo_3x4.png",
    TRIADOR_COLAB_PATH "ana_3x4.png",
    TRIADOR_COLAB_PATH "augusto_3x4.png",
};

// ─── Dados fixos dos colaboradores ────────────────────────────────────────────

typedef struct ColaboradorDado {
    const char      *nome;
    const char      *id;
    const char      *cargo;
    SetorColaborador setor;
} ColaboradorDado;

static const ColaboradorDado COLABORADORES_DADOS[TRIADOR_TOTAL_COLABORADORES] = {
    { "DRA. ELENA VOSS",    "NX-0115", "CIENTISTA CHEFE",              SETOR_PESQUISA        },
    { "DRA. PRIYA NANDAN",  "NX-0223", "CIENTISTA COORIENTADORA",      SETOR_PESQUISA        },
    { "GABRIEL ALMEIDA",    "NX-0291", "CIENTISTA DE DADOS",           SETOR_PESQUISA        },
    { "MARCO ALINE",        "NX-0102", "ENGENHEIRO DE INFRAESTRUTURA", SETOR_TECNOLOGIA      },
    { "OLIVIA DUARTE",      "NX-0047", "ENGENHEIRA DE IA",             SETOR_TECNOLOGIA      },
    { "TEO VASQUES",        "NX-0309", "ANALISTA DE DADOS (JUNIOR)",   SETOR_APOIO_OPERACOES },
    { "ANA REIS",           "NX-0808", "ARQUITETA DE SOLUCOES EM IA",  SETOR_APOIO_OPERACOES },
    { "AUGUSTO FERRAZ",     "NX-003",  "ENGENHEIRO DE MACHINE LEARNING", SETOR_APOIO_OPERACOES },
};

// ─── Constantes de layout ─────────────────────────────────────────────────────

#define LISTA_VISIVEIS      4
#define SLOTS_POR_LINHA     3

// ─── Update helpers ───────────────────────────────────────────────────────────

static void TriadorUpdatePrincipal(TriadorBody *self) {
    // animação do indicador
    self->indicador_timer += GetFrameTime();
    if (self->indicador_timer >= 0.5f) {
        self->indicador_timer  = 0.0f;
        self->indicador_offset = !self->indicador_offset;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        self->cancelado = true;
        return;
    }

    if (self->foco == FOCO_LISTA) {
        // navegar lista
        if (IsKeyPressed(KEY_DOWN)) {
            if (self->cursor < TRIADOR_TOTAL_COLABORADORES - 1) {
                self->cursor++;
                if (self->cursor >= self->scroll_offset + LISTA_VISIVEIS)
                    self->scroll_offset++;
            }
        }
        if (IsKeyPressed(KEY_UP)) {
            if (self->cursor > 0) {
                self->cursor--;
                if (self->cursor < self->scroll_offset)
                    self->scroll_offset--;
            }
        }

        // selecionar / desselecionar
        if (IsKeyPressed(KEY_ENTER)) {
            Colaborador *c = &self->colaboradores[self->cursor];
            if (c->selecionado) {
                c->selecionado = false;
                self->total_selecionados--;
                self->por_setor[c->setor]--;
            } else if (self->total_selecionados < TRIADOR_MAX_SELECIONADOS) {
                c->selecionado = true;
                self->total_selecionados++;
                self->por_setor[c->setor]++;
            }
        }

        // ir para o botão
        if (IsKeyPressed(KEY_RIGHT)) {
            self->foco = FOCO_BOTAO;
        }

    } else { // FOCO_BOTAO
        // voltar para a lista
        if (IsKeyPressed(KEY_LEFT)) {
            self->foco = FOCO_LISTA;
        }

        // confirmar treino
        if (IsKeyPressed(KEY_ENTER)) {
            if (self->total_selecionados < TRIADOR_MAX_SELECIONADOS) {
                self->aviso_timer = 3.0f;
            } else {
                bool correto = true;
                for (int i = 0; i < TRIADOR_TOTAL_SETORES; i++) {
                    if (self->por_setor[i] != TRIADOR_POR_SETOR) {
                        correto = false;
                        break;
                    }
                }
                if (correto) {
                    self->tela_atual = TELA_TRIADOR_ACERTO;
                } else {
                    self->tela_atual = TELA_TRIADOR_ERRO;
                    self->foco = FOCO_LISTA;
                }
            }
        }
        // decrementa aviso
        if (self->aviso_timer > 0.0f)
            self->aviso_timer -= GetFrameTime();
    }
}

static void TriadorUpdateAcerto(TriadorBody *self) {
    if (IsKeyPressed(KEY_ENTER)) {
        self->concluido = true;
    }
}

static void TriadorUpdateErro(TriadorBody *self) {
    // qualquer tecla volta para a tela principal e reseta seleção
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        // reseta seleções
        for (int i = 0; i < TRIADOR_TOTAL_COLABORADORES; i++)
            self->colaboradores[i].selecionado = false;
        self->total_selecionados = 0;
        for (int i = 0; i < TRIADOR_TOTAL_SETORES; i++)
            self->por_setor[i] = 0;
        self->tela_atual = TELA_TRIADOR_PRINCIPAL;
    }
}

// ─── Draw helpers ─────────────────────────────────────────────────────────────

static void TriadorDrawLista(TriadorBody *self, ResourcesBody *resources,
                              int screen_width, int screen_height) {
    float ref_w = 1920.0f, ref_h = 1080.0f;
    float sw = (float)screen_width, sh = (float)screen_height;

    float item_x = sw * (186.0f  / ref_w);
    float item_y = sh * (305.0f  / ref_h);
    float item_w = sw * (474.51f / ref_w);
    float item_h = sh * (158.0f  / ref_h);
    float gap    = sh * (10.0f   / ref_h);

    for (int i = 0; i < LISTA_VISIVEIS; i++) {
        int idx = self->scroll_offset + i;
        if (idx >= TRIADOR_TOTAL_COLABORADORES) break;

        Colaborador *c = &self->colaboradores[idx];
        float x = item_x;
        float y = item_y + i * (item_h + gap);

        DrawTexturePro(
            c->avatar,
            (Rectangle){ 0, 0, (float)c->avatar.width, (float)c->avatar.height },
            (Rectangle){ x, y, item_w, item_h },
            (Vector2){ 0, 0 }, 0.0f, WHITE
        );

        // indicador ao lado do card destacado
        if (idx == self->cursor) {
            float ind_w  = item_h * 0.4f;  // proporcional à altura do card
            float ind_h  = item_h * 0.4f;
            float offset = self->indicador_offset ? 5.0f : 0.0f;
            float ind_x  = x + item_w + 8.0f + offset;
            float ind_y  = y + (item_h - ind_h) / 2.0f;

            DrawTexturePro(
                self->indicador,
                (Rectangle){ 0, 0,
                    (float)self->indicador.width,
                    (float)self->indicador.height },
                (Rectangle){ ind_x, ind_y, ind_w, ind_h },
                (Vector2){ 0, 0 }, 0.0f, WHITE
            );
        }

        if (c->selecionado) {
            DrawRectangleLinesEx((Rectangle){ x, y, item_w, item_h }, 3.0f, resources->letra_caixa);
        }
    }

    // indicadores de scroll
    float meio_x = item_x + item_w / 2.0f - 8.0f;
    if (self->scroll_offset > 0)
        DrawTextEx(resources->fonte_principal, "▲",
            (Vector2){ meio_x, item_y - 24 }, 18, 1, WHITE);
    if (self->scroll_offset + LISTA_VISIVEIS < TRIADOR_TOTAL_COLABORADORES)
        DrawTextEx(resources->fonte_principal, "▼",
            (Vector2){ meio_x, item_y + LISTA_VISIVEIS * (item_h + gap) + 4 }, 18, 1, WHITE);
}

static void TriadorDrawBase(TriadorBody *self, int screen_width, int screen_height) {
    float ref_w = 1920.0f, ref_h = 1080.0f;
    float sw = (float)screen_width, sh = (float)screen_height;

    float slot_x  = sw * (878.0f / ref_w);
    float slot_y  = sh * (423.0f / ref_h);
    float slot_w  = sw * (126.0f / ref_w);
    float slot_h  = sh * (124.0f / ref_h);
    float gap_x   = sw * (18.0f  / ref_w);
    float gap_y   = sh * (15.0f  / ref_h);

    int slot = 0;
    for (int i = 0; i < TRIADOR_TOTAL_COLABORADORES && slot < TRIADOR_MAX_SELECIONADOS; i++) {
        if (!self->colaboradores[i].selecionado) continue;

        int col = slot % SLOTS_POR_LINHA;
        int row = slot / SLOTS_POR_LINHA;
        float sx = slot_x + col * (slot_w + gap_x);
        float sy = slot_y + row * (slot_h + gap_y);

        DrawTexturePro(
            self->colaboradores[i].avatar_3x4,
            (Rectangle){ 0, 0,
                (float)self->colaboradores[i].avatar_3x4.width,
                (float)self->colaboradores[i].avatar_3x4.height },
            (Rectangle){ sx, sy, slot_w, slot_h },
            (Vector2){ 0, 0 }, 0.0f, WHITE
        );
        slot++;
    }

    for (; slot < TRIADOR_MAX_SELECIONADOS; slot++) {
        int col = slot % SLOTS_POR_LINHA;
        int row = slot / SLOTS_POR_LINHA;
        float sx = slot_x + col * (slot_w + gap_x);
        float sy = slot_y + row * (slot_h + gap_y);

        DrawTexturePro(
            self->campo_vazio,
            (Rectangle){ 0, 0,
                (float)self->campo_vazio.width,
                (float)self->campo_vazio.height },
            (Rectangle){ sx, sy, slot_w, slot_h },
            (Vector2){ 0, 0 }, 0.0f, WHITE
        );
    }
}

static void TriadorDrawDistribuicao(TriadorBody *self, ResourcesBody *resources,
                                     int screen_width, int screen_height) {
    float sx = (float)screen_width;
    float sy = (float)screen_height;
    float ref_w = 1920.0f;
    float ref_h = 1080.0f;

    // coordenadas de referência (sem escala) dos números
    // pesquisa: 1712,521 | tecnologia: 1712,584 | apoio: 1712,647 | total: 1600,772
    Vector2 pos_pesquisa   = { sx * (1712.0f / ref_w), sy * (499.0f / ref_h) };
    Vector2 pos_tecnologia = { sx * (1712.0f / ref_w), sy * (562.0f / ref_h) };
    Vector2 pos_apoio      = { sx * (1712.0f / ref_w), sy * (625.0f / ref_h) };
    Vector2 pos_total      = { sx * (1610.0f / ref_w), sy * (750.0f / ref_h) };

    float font_size = sy * (60.0f / ref_h);

    DrawTextEx(resources->fonte_principal,
        TextFormat("%d", self->por_setor[SETOR_PESQUISA]),
        pos_pesquisa, font_size, 1, resources->ciano_claro);

    DrawTextEx(resources->fonte_principal,
        TextFormat("%d", self->por_setor[SETOR_TECNOLOGIA]),
        pos_tecnologia, font_size, 1, resources->ciano_claro);

    DrawTextEx(resources->fonte_principal,
        TextFormat("%d", self->por_setor[SETOR_APOIO_OPERACOES]),
        pos_apoio, font_size, 1, resources->ciano_claro);

    DrawTextEx(resources->fonte_principal,
        TextFormat("%d/%d", self->total_selecionados, TRIADOR_MAX_SELECIONADOS),
        pos_total, font_size, 1, resources->ciano_claro);

}

static void TriadorDrawBotao(TriadorBody *self, ResourcesBody *resources,
                              int screen_width, int screen_height) {
    float sw = (float)screen_width;
    float sh = (float)screen_height;
    float ref_w = 1920.0f, ref_h = 1080.0f;

    // posição e tamanho proporcionais ao fundo
    float bx = sw * (864.0f   / ref_w);
    float by = sh * (797.0f   / ref_h);
    float bw = sw * (578.07f  / ref_w);
    float bh = sh * (101.0f   / ref_h);

    bool ativo = self->total_selecionados == TRIADOR_MAX_SELECIONADOS;
    Color tint = ativo ? WHITE : resources->branco;

    // aviso de seleção incompleta
    if (self->aviso_timer > 0.0f) {
        float alpha = self->aviso_timer < 1.0f
            ? (unsigned char)(self->aviso_timer * 255.0f)
            : 255;
        const char *aviso = "SELECIONE 6 COLABORADORES ANTES DE TREINAR";
        Vector2 tam = MeasureTextEx(resources->fonte_principal, aviso, 32, 1);
        DrawTextEx(resources->fonte_principal, aviso,
            (Vector2){ bx + (bw - tam.x) / 2.0f, by - 30.0f },
            25, 1, (Color){ 255, 80, 80, (unsigned char)alpha });
    }

    Texture2D tex_botao = (self->foco == FOCO_BOTAO) ? self->botao_selecionado : self->botao;
    DrawTexturePro(
        tex_botao,
        (Rectangle){ 0, 0, (float)self->botao.width, (float)self->botao.height },
        (Rectangle){ bx, by, bw, bh },
        (Vector2){ 0, 0 }, 0.0f, tint
    );
}

static void TriadorDrawPrincipal(TriadorBody *self, ResourcesBody *resources,
                                  int screen_width, int screen_height) {
    DrawTexturePro(
        self->background,
        (Rectangle){ 0, 0, (float)self->background.width, (float)self->background.height },
        (Rectangle){ 0, 0, screen_width, screen_height },
        (Vector2){ 0, 0 }, 0.0f, WHITE
    );

    TriadorDrawLista(self, resources, screen_width, screen_height);
    TriadorDrawBase(self, screen_width, screen_height);
    TriadorDrawDistribuicao(self, resources, screen_width, screen_height);
    TriadorDrawBotao(self, resources, screen_width, screen_height);
}

static void TriadorDrawErro(TriadorBody *self, ResourcesBody *resources,
                             int screen_width, int screen_height) {
    DrawRectangle(0, 0, screen_width, screen_height, (Color){ 0, 0, 0, 180 });

    float sw = (float)screen_width, sh = (float)screen_height;
    float cw = sw * (960.0f / 1920.0f);
    float ch = sh * (600.0f / 1080.0f);
    float cx = (sw - cw) / 2.0f;
    float cy = (sh - ch) / 2.0f;

    DrawRectangleRounded(
        (Rectangle){ cx, cy, cw, ch }, 0.1f, 8,
        (Color){ 20, 10, 40, 240 }
    );
    DrawRectangleRoundedLinesEx(
        (Rectangle){ cx, cy, cw, ch }, 0.1f, 8, 2.0f, RED
    );

    const char *titulo = "TREINAMENTO INVALIDO";
    const char *desc1  = "A base de treinamento nao esta balanceada.";
    const char *desc2  = "Cada setor deve ter exatamente 2 colaboradores.";
    const char *desc3  = "Vies nos dados compromete o comportamento da IA.";
    const char *aviso  = "PENALIDADE: -15 SEGUNDOS";
    const char *sair   = "[ENTER] TENTAR NOVAMENTE";

    float t_size = sh * (28.0f / 1080.0f);
    float d_size = sh * (22.0f / 1080.0f);
    float a_size = sh * (24.0f / 1080.0f);

    float meio_x = cx + cw / 2.0f;

    DrawTextEx(resources->fonte_principal, titulo,
        (Vector2){ meio_x - MeasureTextEx(resources->fonte_principal, titulo, t_size, 1).x / 2.0f,
                   cy + ch * 0.08f }, t_size, 1, RED);

    DrawTextEx(resources->fonte_principal, desc1,
        (Vector2){ cx + cw * 0.07f, cy + ch * 0.28f }, d_size, 1, LIGHTGRAY);
    DrawTextEx(resources->fonte_principal, desc2,
        (Vector2){ cx + cw * 0.07f, cy + ch * 0.38f }, d_size, 1, LIGHTGRAY);
    DrawTextEx(resources->fonte_principal, desc3,
        (Vector2){ cx + cw * 0.07f, cy + ch * 0.48f }, d_size, 1, LIGHTGRAY);

    DrawTextEx(resources->fonte_principal, aviso,
        (Vector2){ meio_x - MeasureTextEx(resources->fonte_principal, aviso, a_size, 1).x / 2.0f,
                   cy + ch * 0.65f }, a_size, 1, ORANGE);

    DrawTextEx(resources->fonte_principal, sair,
        (Vector2){ meio_x - MeasureTextEx(resources->fonte_principal, sair, d_size, 1).x / 2.0f,
                   cy + ch * 0.82f }, d_size, 1, WHITE);
}

static void TriadorDrawAcerto(TriadorBody *self, ResourcesBody *resources,
                               int screen_width, int screen_height) {
    DrawRectangle(0, 0, screen_width, screen_height, (Color){ 0, 0, 0, 180 });

    float sw = (float)screen_width, sh = (float)screen_height;
    float cw = sw * (960.0f / 1920.0f);
    float ch = sh * (600.0f / 1080.0f);
    float cx = (sw - cw) / 2.0f;
    float cy = (sh - ch) / 2.0f;

    DrawRectangleRounded(
        (Rectangle){ cx, cy, cw, ch }, 0.1f, 8,
        (Color){ 10, 30, 20, 240 }
    );
    DrawRectangleRoundedLinesEx(
        (Rectangle){ cx, cy, cw, ch }, 0.1f, 8, 2.0f, resources->letra_caixa
    );

    const char *titulo = "TREINAMENTO CONCLUIDO";
    const char *desc1  = "A base de treinamento esta balanceada.";
    const char *desc2  = "2 colaboradores de cada setor foram selecionados.";
    const char *desc3  = "A IA sera treinada com dados representativos.";
    const char *sair   = "[ENTER] CONTINUAR";

    float t_size = sh * (28.0f / 1080.0f);
    float d_size = sh * (22.0f / 1080.0f);

    float meio_x = cx + cw / 2.0f;

    DrawTextEx(resources->fonte_principal, titulo,
        (Vector2){ meio_x - MeasureTextEx(resources->fonte_principal, titulo, t_size, 1).x / 2.0f,
                   cy + ch * 0.08f }, t_size, 1, resources->letra_caixa);

    DrawTextEx(resources->fonte_principal, desc1,
        (Vector2){ cx + cw * 0.07f, cy + ch * 0.28f }, d_size, 1, LIGHTGRAY);
    DrawTextEx(resources->fonte_principal, desc2,
        (Vector2){ cx + cw * 0.07f, cy + ch * 0.38f }, d_size, 1, LIGHTGRAY);
    DrawTextEx(resources->fonte_principal, desc3,
        (Vector2){ cx + cw * 0.07f, cy + ch * 0.48f }, d_size, 1, LIGHTGRAY);

    DrawTextEx(resources->fonte_principal, sair,
        (Vector2){ meio_x - MeasureTextEx(resources->fonte_principal, sair, d_size, 1).x / 2.0f,
                   cy + ch * 0.82f }, d_size, 1, WHITE);
}

// ─── API pública ──────────────────────────────────────────────────────────────

void TriadorInit(TriadorBody *self) {
    self->instrucoes = LoadTexture(TRIADOR_PATH "minigame01.png");
    self->mostrando_instrucoes = true;
    self->tela_atual         = TELA_TRIADOR_PRINCIPAL;
    self->foco = FOCO_LISTA;
    self->cursor             = 0;
    self->scroll_offset      = 0;
    self->total_selecionados = 0;
    self->concluido          = false;
    self->cancelado          = false;

    for (int i = 0; i < TRIADOR_TOTAL_SETORES; i++)
        self->por_setor[i] = 0;

    // carrega assets gerais
    self->background = LoadTexture(TRIADOR_PATH "triador_sem_contas.png");
    self->campo_vazio = LoadTexture(TRIADOR_PATH "campo_vazio.png");
    self->botao            = LoadTexture(TRIADOR_PATH "botao.png");
    self->botao_selecionado = LoadTexture(TRIADOR_PATH "botao_selecionado.png");
    self->aviso_timer = 0.0f;
    self->indicador        = LoadTexture(TRIADOR_PATH "polygon_cursor.png");
    self->indicador_timer  = 0.0f;
    self->indicador_offset = false;
    
    // carrega colaboradores
    for (int i = 0; i < TRIADOR_TOTAL_COLABORADORES; i++) {
        self->colaboradores[i].nome      = COLABORADORES_DADOS[i].nome;
        self->colaboradores[i].id        = COLABORADORES_DADOS[i].id;
        self->colaboradores[i].cargo     = COLABORADORES_DADOS[i].cargo;
        self->colaboradores[i].setor     = COLABORADORES_DADOS[i].setor;
        self->colaboradores[i].selecionado = false;
        self->colaboradores[i].avatar    = LoadTexture(AVATAR_CAMINHOS[i]);
        self->colaboradores[i].avatar_3x4 = LoadTexture(AVATAR_3X4_CAMINHOS[i]);
    }

    self->objeto = InteragivelCriar(1066 * MAP_ESCALA, 1695 * MAP_ESCALA, (1143-1066) * MAP_ESCALA, (1824-1695) * MAP_ESCALA, 40.0f);
}

void TriadorUpdate(TriadorBody *self) {
    if (self->mostrando_instrucoes) {
        if (IsKeyPressed(KEY_ESCAPE)) self->cancelado = true;
        else if (IsKeyPressed(KEY_ENTER)) self->mostrando_instrucoes = false;
        return;
    }
    switch (self->tela_atual) {
        case TELA_TRIADOR_PRINCIPAL: TriadorUpdatePrincipal(self); break;
        case TELA_TRIADOR_ERRO:      TriadorUpdateErro(self);      break;
        case TELA_TRIADOR_ACERTO:    TriadorUpdateAcerto(self);    break;
    }
}

void TriadorDrawIndicador(TriadorBody *self, ResourcesBody *resources, Camera2D camera) {
    if (!self->objeto.ativo) return;

    Vector2 pos_tela = GetWorldToScreen2D(
        (Vector2){ self->objeto.mundo_rect.x + self->objeto.mundo_rect.width / 2.0f,
                   self->objeto.mundo_rect.y - 40.0f },
        camera
    );

    const char *texto = "E - interagir";
    float tamanho = 36.0f;
    Vector2 dim = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);

    bool visivel = ((int)(GetTime() * 2) % 2 == 0);
    Color cor = visivel ? resources->amarelo : resources->lilas_claro;

    DrawTextEx(resources->fonte_principal, texto,
        (Vector2){ pos_tela.x - dim.x / 2.0f, pos_tela.y },
        tamanho, 1, cor);
}

void TriadorDraw(TriadorBody *self, ResourcesBody *resources,
                 int screen_width, int screen_height) {
    if (self->mostrando_instrucoes) {
        InstrucoesDraw(self->instrucoes, resources, screen_width, screen_height);
        return;
    }
    switch (self->tela_atual) {
        case TELA_TRIADOR_PRINCIPAL:
            TriadorDrawPrincipal(self, resources, screen_width, screen_height);
            break;
        case TELA_TRIADOR_ERRO:
            TriadorDrawPrincipal(self, resources, screen_width, screen_height);
            TriadorDrawErro(self, resources, screen_width, screen_height);
            break;
        case TELA_TRIADOR_ACERTO:
            TriadorDrawPrincipal(self, resources, screen_width, screen_height);
            TriadorDrawAcerto(self, resources, screen_width, screen_height);
            break;
    }
}

void TriadorUnload(TriadorBody *self) {
    UnloadTexture(self->instrucoes);
    UnloadTexture(self->background);
    UnloadTexture(self->campo_vazio);
    UnloadTexture(self->botao);
    UnloadTexture(self->indicador);
    for (int i = 0; i < TRIADOR_TOTAL_COLABORADORES; i++) {
        UnloadTexture(self->colaboradores[i].avatar);
        UnloadTexture(self->colaboradores[i].avatar_3x4);
    }
}