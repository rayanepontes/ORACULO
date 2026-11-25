/** @file minigame_reconector.c
 * @brief Implementação do minigame Reconector Neural.
 */
#include "scenes/minigame_reconector.h"
#include "graphics/instrucoes.h"
#include "map/map.h"
#include <math.h>
#include <string.h>

#define RECONECTOR_PATH "assets/texture/minigame_reconector/"

// ─── Layout de referência (1920×1080) ────────────────────────────────────────
#define REF_W        1920.0f
#define REF_H        1080.0f
#define GRID_X0      722.0f
#define GRID_Y0      262.0f
#define GRID_W       640.0f
#define GRID_H       640.0f

// Botão resetar no design
#define BOTAO_X     1455.0f
#define BOTAO_Y      399.63f
#define BOTAO_W      224.0f
#define BOTAO_H      277.37f

// ─── Posições fixas dos módulos no grid ──────────────────────────────────────
typedef struct { int col, lin, par; } AncoraDef;

static const AncoraDef ANCORAS[] = {
    { 0, 7, PAR_VISAO    },  { 1, 7, PAR_VISAO    },   // câmera + chip azul
    { 0, 0, PAR_VOZ      },  { 2, 2, PAR_VOZ      },   // microfone + chip verde
    { 7, 0, PAR_LLM      },  { 7, 3, PAR_LLM      },   // chip amarelo + teclado
    { 2, 5, PAR_FIREWALL },  { 6, 5, PAR_FIREWALL },   // escudo + chip vermelho
    { 1, 2, PAR_ML       },  { 2, 3, PAR_ML       },   // rede de nós + chip ciano
    { 0, 1, PAR_OPTICO   },  { 7, 7, PAR_OPTICO   },   // chip roxo + olho
    { 3, 6, PAR_REDE     },  { 4, 6, PAR_REDE     },   // globo + chip laranja
    { 2, 0, PAR_TTS      },  { 4, 2, PAR_TTS      },   // documento + chip rosa
};

#define TOTAL_ANCORAS ((int)(sizeof(ANCORAS) / sizeof(ANCORAS[0])))

// ─── Helpers ──────────────────────────────────────────────────────────────────

static bool TelaParaGrid(Vector2 pos, float sw, float sh, int *col_out, int *lin_out) {
    float escala_x = sw / REF_W;
    float escala_y = sh / REF_H;
    float ox = GRID_X0 * escala_x;
    float oy = GRID_Y0 * escala_y;
    float gw = GRID_W  * escala_x;
    float gh = GRID_H  * escala_y;

    if (pos.x < ox || pos.x >= ox + gw) return false;
    if (pos.y < oy || pos.y >= oy + gh) return false;

    float cell_w = gw / RECONECTOR_GRID_COLS;
    float cell_h = gh / RECONECTOR_GRID_ROWS;
    *col_out = (int)((pos.x - ox) / cell_w);
    *lin_out = (int)((pos.y - oy) / cell_h);
    return true;
}

static void ReconectorResetarGrid(ReconectorBody *self) {
    for (int c = 0; c < RECONECTOR_GRID_COLS; c++)
        for (int l = 0; l < RECONECTOR_GRID_ROWS; l++)
            self->grid[c][l] = (CelulaReconector){ RECONECTOR_VAZIO, false };

    for (int i = 0; i < RECONECTOR_TOTAL_PARES; i++) {
        self->cabo_tamanho[i] = 0;
        self->par_concluido[i] = false;
    }

    for (int i = 0; i < TOTAL_ANCORAS; i++) {
        int c = ANCORAS[i].col;
        int l = ANCORAS[i].lin;
        self->grid[c][l] = (CelulaReconector){ ANCORAS[i].par, true };
    }

    self->arrastando_par = -1;
}

static void ApagarCabo(ReconectorBody *self, int par) {
    for (int c = 0; c < RECONECTOR_GRID_COLS; c++)
        for (int l = 0; l < RECONECTOR_GRID_ROWS; l++)
            if (self->grid[c][l].par == par && !self->grid[c][l].ancora)
                self->grid[c][l] = (CelulaReconector){ RECONECTOR_VAZIO, false };
    self->cabo_tamanho[par] = 0;
    self->par_concluido[par] = false;
}

/** @brief Acrescenta uma célula ao fim do caminho do cabo e move o ponto de arrasto para ela. */
static void CaboAdicionar(ReconectorBody *self, int par, int col, int lin) {
    int n = self->cabo_tamanho[par];
    if (n >= RECONECTOR_GRID_COLS * RECONECTOR_GRID_ROWS) return;

    self->caminho_col[par][n] = col;
    self->caminho_lin[par][n] = lin;
    self->cabo_tamanho[par]   = n + 1;
    self->arrastar_col        = col;
    self->arrastar_lin        = lin;
}

static bool EhAdjacente(int col, int lin, int col_prev, int lin_prev) {
    int dc = col - col_prev;
    int dl = lin - lin_prev;
    return (dc == 0 && (dl == 1 || dl == -1)) ||
           (dl == 0 && (dc == 1 || dc == -1));
}

static bool TodosConcluidos(ReconectorBody *self) {
    for (int i = 0; i < RECONECTOR_TOTAL_PARES; i++)
        if (!self->par_concluido[i]) return false;
    return true;
}

// ─── Update ───────────────────────────────────────────────────────────────────

/** @brief Marca o par como conectado e verifica se o minigame acabou. */
static void ReconectorConcluirPar(ReconectorBody *self, ResourcesBody *resources,
                                  int par, int col, int lin) {
    CaboAdicionar(self, par, col, lin);
    self->par_concluido[par] = true;
    self->arrastando_par = -1;
    PlaySound(resources->som_confirmacao);
    if (TodosConcluidos(self)) {
        self->tela_atual = TELA_RECONECTOR_ACERTO;
    }
}

static void ReconectorUpdateJogo(ReconectorBody *self, ResourcesBody *resources,
                                  float sw, float sh) {
    Vector2 mouse = GetMousePosition();

    // ── Hover do botão resetar ────────────────────────────────────────────────
    bool novo_hover = CheckCollisionPointRec(mouse, self->rect_botao_resetar);
    if (novo_hover && !self->hover_botao_resetar)
        PlaySound(resources->som_selecao);
    self->hover_botao_resetar = novo_hover;

    if (novo_hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        PlaySound(resources->som_confirmacao);
        ReconectorResetarGrid(self);
        self->arrastando_par = -1;
        return;
    }

    // ── Início do arrasto ─────────────────────────────────────────────────────
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int col, lin;
        if (TelaParaGrid(mouse, sw, sh, &col, &lin)) {
            if (self->grid[col][lin].ancora) {
                int par = self->grid[col][lin].par;
                ApagarCabo(self, par);
                self->arrastando_par = par;
                CaboAdicionar(self, par, col, lin);
                self->origem_col = col;
                self->origem_lin = lin;
                PlaySound(resources->som_selecao);
            }
        }
    }

    // ── Soltar o mouse ────────────────────────────────────────────────────────
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        int par = self->arrastando_par;

        // Se o cabo já andou e parou colado no módulo de destino, conclui o par
        if (par != -1 && self->cabo_tamanho[par] > 1) {
            int dc[] = { 1, -1, 0, 0 };
            int dl[] = { 0, 0, 1, -1 };
            for (int d = 0; d < 4; d++) {
                int nc = self->arrastar_col + dc[d];
                int nl = self->arrastar_lin + dl[d];
                if (nc < 0 || nc >= RECONECTOR_GRID_COLS) continue;
                if (nl < 0 || nl >= RECONECTOR_GRID_ROWS) continue;

                CelulaReconector *viz = &self->grid[nc][nl];
                if (viz->ancora && viz->par == par &&
                    !(nc == self->origem_col && nl == self->origem_lin)) {
                    ReconectorConcluirPar(self, resources, par, nc, nl);
                    break;
                }
            }
        }
        self->arrastando_par = -1;
    }

    // ── Arrasto em andamento ──────────────────────────────────────────────────
    if (self->arrastando_par == -1) return;
    if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) return;

    int col, lin;
    if (!TelaParaGrid(mouse, sw, sh, &col, &lin)) return;

    if (col == self->arrastar_col && lin == self->arrastar_lin) return;
    if (!EhAdjacente(col, lin, self->arrastar_col, self->arrastar_lin)) return;

    int par = self->arrastando_par;
    CelulaReconector *cel = &self->grid[col][lin];

    // Célula vazia → preenche com cabo
    if (cel->par == RECONECTOR_VAZIO) {
        cel->par    = par;
        cel->ancora = false;
        CaboAdicionar(self, par, col, lin);
        return;
    }

    // Âncora do mesmo par → conclui, a menos que seja a de onde o cabo saiu
    if (cel->ancora && cel->par == par) {
        if (col == self->origem_col && lin == self->origem_lin) return;
        ReconectorConcluirPar(self, resources, par, col, lin);
        return;
    }

    // Cabo de outro par → apaga aquele cabo e preenche com o atual
    if (!cel->ancora && cel->par != RECONECTOR_VAZIO && cel->par != par) {
        ApagarCabo(self, cel->par);
        cel->par    = par;
        cel->ancora = false;
        CaboAdicionar(self, par, col, lin);
        return;
    }
}

static void ReconectorUpdateAcerto(ReconectorBody *self, ResourcesBody *resources) {
    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        PlaySound(resources->som_confirmacao);
        self->concluido = true;
    }
}

// ─── Draw ─────────────────────────────────────────────────────────────────────

static void ReconectorDrawGrid(ReconectorBody *self, float sw, float sh) {
    float escala_x = sw / REF_W;
    float escala_y = sh / REF_H;
    float ox       = GRID_X0 * escala_x;
    float oy       = GRID_Y0 * escala_y;
    float cell_w   = (GRID_W / RECONECTOR_GRID_COLS) * escala_x;
    float cell_h   = (GRID_H / RECONECTOR_GRID_ROWS) * escala_y;

    float cabo_w = cell_w * 0.35f;
    float cabo_h = cell_h * 0.35f;

    // ── Cabos ────────────────────────────────────────────────────────────────
    // ── Cabos: percorre o caminho de cada par, na ordem em que foi desenhado ──
    for (int par = 0; par < RECONECTOR_TOTAL_PARES; par++) {
        int   n   = self->cabo_tamanho[par];
        Color cor = self->cores[par];

        for (int i = 0; i < n; i++) {
            int   c  = self->caminho_col[par][i];
            int   l  = self->caminho_lin[par][i];
            float cx = ox + c * cell_w + cell_w / 2.0f;
            float cy = oy + l * cell_h + cell_h / 2.0f;

            // Bolinha (as âncoras ficam sem, para não cobrir o ícone do fundo)
            if (!self->grid[c][l].ancora)
                DrawCircle((int)cx, (int)cy, cabo_w / 2.0f, cor);

            // Barra ligando esta célula à anterior do caminho
            if (i > 0) {
                int   pc = self->caminho_col[par][i - 1];
                int   pl = self->caminho_lin[par][i - 1];
                float px = ox + pc * cell_w + cell_w / 2.0f;
                float py = oy + pl * cell_h + cell_h / 2.0f;

                float rx = (cx + px) / 2.0f;
                float ry = (cy + py) / 2.0f;
                float rw = (c != pc) ? (cell_w / 2.0f + 1) : cabo_w;
                float rh = (l != pl) ? (cell_h / 2.0f + 1) : cabo_h;
                DrawRectangle((int)(rx - rw / 2.0f), (int)(ry - rh / 2.0f),
                              (int)rw, (int)rh, cor);
            }
        }
    }
}

/** @brief Desenha um quadradinho por par: cheio quando conectado, só contorno quando não. */
static void ReconectorDrawProgresso(ReconectorBody *self, float sw, float sh) {
    float ex = sw / REF_W;
    float ey = sh / REF_H;
    float lado = 20.0f, gap = 12.0f;
    float x0 = GRID_X0 * ex;
    float y0 = 912.0f * ey;

    for (int i = 0; i < RECONECTOR_TOTAL_PARES; i++) {
        Rectangle r = { x0 + i * (lado + gap) * ex, y0, lado * ex, lado * ey };
        if (self->par_concluido[i]) DrawRectangleRec(r, self->cores[i]);
        else                        DrawRectangleLinesEx(r, 2.0f, self->cores[i]);
    }
}

static void ReconectorDrawJogo(ReconectorBody *self, ResourcesBody *resources,
                                int sw, int sh) {
    // Fundo
    DrawTexturePro(self->background,
        (Rectangle){ 0, 0, (float)self->background.width, (float)self->background.height },
        (Rectangle){ 0, 0, (float)sw, (float)sh },
        (Vector2){ 0, 0 }, 0.0f, WHITE);

    ReconectorDrawGrid(self, (float)sw, (float)sh);
    ReconectorDrawProgresso(self, (float)sw, (float)sh);

    // Botão resetar
    Texture2D tex_btn = self->hover_botao_resetar
                        ? self->botao_resetar_sel
                        : self->botao_resetar;
    if (tex_btn.id != 0) {
        DrawTexturePro(tex_btn,
            (Rectangle){ 0, 0, (float)tex_btn.width, (float)tex_btn.height },
            self->rect_botao_resetar,
            (Vector2){ 0, 0 }, 0.0f, WHITE);
    }
}

static void ReconectorDrawAcerto(ReconectorBody *self, ResourcesBody *resources,
                                  int sw, int sh) {
    ReconectorDrawJogo(self, resources, sw, sh);

    DrawRectangle(0, 0, sw, sh, (Color){ 0, 0, 0, 200 });
    DrawRectangle(0, 0, sw, sh, (Color){ 0, 128, 0, 60 });

    float cx = sw / 2.0f;

    const char *titulo = "RECONEXAO CONCLUIDA";
    float t_size = sh * (60.0f / REF_H);
    Vector2 dt = MeasureTextEx(resources->fonte_principal, titulo, t_size, 1);
    DrawTextEx(resources->fonte_principal, titulo,
        (Vector2){ cx - dt.x / 2.0f, sh * 0.30f },
        t_size, 1, resources->ciano_claro);

    const char *sub = "Todos os modulos neurais foram reconectados.";
    float s_size = sh * (36.0f / REF_H);
    Vector2 ds = MeasureTextEx(resources->fonte_principal, sub, s_size, 1);
    DrawTextEx(resources->fonte_principal, sub,
        (Vector2){ cx - ds.x / 2.0f, sh * 0.46f },
        s_size, 1, resources->branco);

    bool v = ((int)(GetTime() * 2) % 2 == 0);
    const char *continuar = "ENTER para continuar";
    float c_size = sh * (44.0f / REF_H);
    Vector2 dc = MeasureTextEx(resources->fonte_principal, continuar, c_size, 1);
    DrawTextEx(resources->fonte_principal, continuar,
        (Vector2){ cx - dc.x / 2.0f, sh * 0.70f },
        c_size, 1, v ? resources->amarelo : resources->branco);
}

// ─── API pública ──────────────────────────────────────────────────────────────

void ReconectorInit(ReconectorBody *self) {
    self->instrucoes            = LoadTexture(RECONECTOR_PATH "minigame04.png");
    self->mostrando_instrucoes  = true;
    self->tela_atual            = TELA_RECONECTOR_JOGO;
    self->concluido             = false;
    self->cancelado             = false;
    self->arrastando_par        = -1;
    self->hover_botao_resetar   = false;

    self->background        = LoadTexture(RECONECTOR_PATH "reconector_neural.png");
    self->botao_resetar     = LoadTexture(RECONECTOR_PATH "botao_resetar.png");
    self->botao_resetar_sel = LoadTexture(RECONECTOR_PATH "botao_resetar_selected.png");


    // Cores de cada par — ajuste os hexadecimais conforme o design
    self->cores[PAR_VISAO]    = GetColor(0x2F6BFFFF); // azul
    self->cores[PAR_VOZ]      = GetColor(0x4BE07AFF); // verde
    self->cores[PAR_LLM]      = GetColor(0xF2C230FF); // amarelo
    self->cores[PAR_FIREWALL] = GetColor(0xF0185CFF); // vermelho
    self->cores[PAR_ML]       = GetColor(0x3DF0C8FF); // ciano
    self->cores[PAR_OPTICO]   = GetColor(0xB03BFFFF); // roxo
    self->cores[PAR_REDE]     = GetColor(0xF07A1AFF); // laranja
    self->cores[PAR_TTS]      = GetColor(0xE8409CFF); // rosa

    // Posição do botão resetar (escala pelo tamanho da tela no Init)
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    float ex = (float)sw / REF_W;
    float ey = (float)sh / REF_H;
    self->rect_botao_resetar = (Rectangle){
        BOTAO_X * ex, BOTAO_Y * ey,
        BOTAO_W * ex, BOTAO_H * ey
    };

    // Objeto interagível no mapa — ajuste as coordenadas conforme o mapa
    self->objeto = InteragivelCriar(
        527 * MAP_ESCALA, 750 * MAP_ESCALA,
        (597-527)  * MAP_ESCALA, (869-750)  * MAP_ESCALA,
        40.0f
    );

    ReconectorResetarGrid(self);
}

void ReconectorUpdate(ReconectorBody *self, ResourcesBody *resources) {
    if (self->mostrando_instrucoes) {
        if (IsKeyPressed(KEY_ESCAPE)) self->cancelado = true;
        else if (IsKeyPressed(KEY_ENTER)) self->mostrando_instrucoes = false;
        return;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        self->cancelado = true;
        return;
    }

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Atualiza rect do botão caso a tela tenha mudado de tamanho
    float ex = (float)sw / REF_W;
    float ey = (float)sh / REF_H;
    self->rect_botao_resetar = (Rectangle){
        BOTAO_X * ex, BOTAO_Y * ey,
        BOTAO_W * ex, BOTAO_H * ey
    };

    switch (self->tela_atual) {
        case TELA_RECONECTOR_JOGO:
            ReconectorUpdateJogo(self, resources, (float)sw, (float)sh);
            break;
        case TELA_RECONECTOR_ACERTO:
            ReconectorUpdateAcerto(self, resources);
            break;
    }
}

void ReconectorDrawIndicador(ReconectorBody *self, ResourcesBody *resources, Camera2D camera) {
    if (!self->objeto.ativo) return;

    Vector2 pos_tela = GetWorldToScreen2D(
        (Vector2){ self->objeto.mundo_rect.x + self->objeto.mundo_rect.width  / 2.0f,
                   self->objeto.mundo_rect.y - 40.0f },
        camera);

    const char *texto   = "E - interagir";
    float       tamanho = 36.0f;
    Vector2     dim     = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);
    bool        visivel = ((int)(GetTime() * 2) % 2 == 0);
    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ pos_tela.x - dim.x / 2.0f, pos_tela.y },
               tamanho, 1, visivel ? resources->amarelo : resources->lilas_claro);
}

void ReconectorDraw(ReconectorBody *self, ResourcesBody *resources, int sw, int sh) {
    if (self->mostrando_instrucoes) {
        InstrucoesDraw(self->instrucoes, resources, sw, sh);
        return;
    }
    switch (self->tela_atual) {
        case TELA_RECONECTOR_JOGO:
            ReconectorDrawJogo(self, resources, sw, sh);
            break;
        case TELA_RECONECTOR_ACERTO:
            ReconectorDrawAcerto(self, resources, sw, sh);
            break;
    }
}

void ReconectorUnload(ReconectorBody *self) {
    UnloadTexture(self->instrucoes);
    UnloadTexture(self->background);
    UnloadTexture(self->botao_resetar);
    UnloadTexture(self->botao_resetar_sel);
}