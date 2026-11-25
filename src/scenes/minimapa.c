/**
 * @file minimapa.c
 * @brief Implementação do minimapa de tarefas.
 */

#include "scenes/minimapa.h"
#include <math.h>

/** @brief Nomes das salas (sem acento: a fonte só tem ASCII). */
static const char *MINIMAPA_NOMES[MINIMAPA_TOTAL_SALAS] = {
    "SALA DO COFRE", "ESCRITORIO", "COPA", "RECEPCAO", "UNIDADE DE FORMACAO", "SERVIDORES"
};

/** @brief Centro de cada nome no design (mesma ordem do enum). TODO: ajustar. */
static const Vector2 MINIMAPA_POS_NOME[MINIMAPA_TOTAL_SALAS] = {
    {  364.0f,  500.0f },  // COFRE
    {  1390.0f,  300.0f },  // ESCRITORIO
    { 3442.0f,  664.0f },  // COPA
    { 1815.0f,  1228.0f },  // RECEPCAO
    {  688.0f, 1743.0f },  // FORMACAO
    { 3087.0f, 1764.0f }   // SERVIDORES
};

/** @brief Centro de cada triângulo no design. TODO: ajustar (COPA e RECEPCAO são sugestões). */
static const Vector2 MINIMAPA_POS_TRIANGULO[MINIMAPA_TOTAL_SALAS] = {
    {  364.0f,  685.0f },  // COFRE
    {  1390.0f,  485.0f },  // ESCRITORIO
    { 3135.0f,  341.0f },  // COPA
    { 2590.0f,  1288.0f },  // RECEPCAO
    {  688.0f, 1928.0f },  // FORMACAO
    { 3087.0f, 1949.0f }   // SERVIDORES
};

/** @brief Mistura duas cores (t = 0 devolve a, t = 1 devolve b). */
static Color MinimapaMisturar(Color a, Color b, float t) {
    return (Color){
        (unsigned char)(a.r + (b.r - a.r) * t),
        (unsigned char)(a.g + (b.g - a.g) * t),
        (unsigned char)(a.b + (b.b - a.b) * t),
        255
    };
}

/** @brief Retângulo do mapa na tela, centralizado e mantendo a proporção. */
static Rectangle MinimapaDestinoMapa(int screen_width, int screen_height) {
    float escala = fminf((screen_width  * MINIMAPA_MAPA_MAX_W) / MINIMAPA_REF_W,
                         (screen_height * MINIMAPA_MAPA_MAX_H) / MINIMAPA_REF_H);
    float w = MINIMAPA_REF_W * escala;
    float h = MINIMAPA_REF_H * escala;
    return (Rectangle){ (screen_width - w) / 2.0f, (screen_height - h) / 2.0f, w, h };
}

/** @brief Retângulo do ícone na tela (quadrado, para o círculo não deformar). */
static Rectangle MinimapaRetanguloIcone(int screen_width, int screen_height) {
    float sx = (float)screen_width  / MINIMAPA_TELA_REF_W;
    float sy = (float)screen_height / MINIMAPA_TELA_REF_H;
    float lado = MINIMAPA_ICONE_LADO_REF * fminf(sx, sy);
    float x = screen_width - MINIMAPA_ICONE_MARGEM_REF * sx - lado;
    float y = MINIMAPA_ICONE_Y_REF * sy;
    if (y < MINIMAPA_ICONE_Y_MIN_PX) y = MINIMAPA_ICONE_Y_MIN_PX;
    return (Rectangle){ x, y, lado, lado };
}

void MinimapaInit(MinimapaBody *self) {
    self->base        = LoadTexture(MINIMAPA_BASE_PATH);
    self->triangulo   = LoadTexture(MINIMAPA_TRIANGULO_PATH);
    self->icone_cafe = LoadTexture(MINIMAPA_CAFE_ICON_PATH);
    if (self->icone_cafe.id == 0) TraceLog(LOG_WARNING, "MINIMAPA: icone do cafe nao carregado (%s)", MINIMAPA_CAFE_ICON_PATH);
    self->icone       = LoadTexture(MINIMAPA_ICONE_PATH);
    self->icone_hover = LoadTexture(MINIMAPA_ICONE_HOVER_PATH);  // opcional

    if (self->base.id      == 0) TraceLog(LOG_WARNING, "MINIMAPA: base nao carregada (%s)", MINIMAPA_BASE_PATH);
    if (self->triangulo.id == 0) TraceLog(LOG_WARNING, "MINIMAPA: triangulo nao carregado (%s)", MINIMAPA_TRIANGULO_PATH);
    if (self->icone.id     == 0) TraceLog(LOG_WARNING, "MINIMAPA: icone nao carregado (%s)", MINIMAPA_ICONE_PATH);

    for (int i = 0; i < MINIMAPA_TOTAL_SALAS; i++) {
        self->alpha_alerta[i] = 1.0f;   // começa tudo pendente
    }
}

void MinimapaCalcularPendentes(bool computador_usado, bool triador_usado,
                               bool alavanca_usada, bool reconector_usado,
                               bool botao_usado, bool cafeteira_usada,
                               bool saida[MINIMAPA_TOTAL_SALAS]) {
    saida[MINIMAPA_SALA_COFRE]      = !reconector_usado;
    saida[MINIMAPA_SALA_ESCRITORIO] = !computador_usado;
    saida[MINIMAPA_SALA_COPA]       = !cafeteira_usada;
    saida[MINIMAPA_SALA_RECEPCAO]   = !botao_usado;
    saida[MINIMAPA_SALA_FORMACAO]   = !triador_usado;
    saida[MINIMAPA_SALA_SERVIDORES] = !alavanca_usada;
}

void MinimapaUpdate(MinimapaBody *self, const bool pendentes[MINIMAPA_TOTAL_SALAS]) {
    float passo = MINIMAPA_FADE_VELOCIDADE * GetFrameTime();
    for (int i = 0; i < MINIMAPA_TOTAL_SALAS; i++) {
        float alvo = pendentes[i] ? 1.0f : 0.0f;
        float *a = &self->alpha_alerta[i];
        if (*a < alvo) *a = fminf(*a + passo, alvo);
        else           *a = fmaxf(*a - passo, alvo);
    }
}

bool MinimapaIconeClicado(int screen_width, int screen_height) {
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           CheckCollisionPointRec(GetMousePosition(),
                                  MinimapaRetanguloIcone(screen_width, screen_height));
}

void MinimapaDrawIcone(MinimapaBody *self, int screen_width, int screen_height, bool aberto) {
    Rectangle destino = MinimapaRetanguloIcone(screen_width, screen_height);
    bool hover = aberto || CheckCollisionPointRec(GetMousePosition(), destino);

    if (self->icone.id == 0) {
        // Fallback para testar a área de clique antes de ter a arte
        Color cor = hover ? (Color){ 239, 178, 57, 255 } : (Color){ 153, 136, 187, 255 };
        DrawCircle((int)(destino.x + destino.width / 2.0f),
                   (int)(destino.y + destino.height / 2.0f), destino.width / 2.0f, cor);
        return;
    }

    Texture2D tex = (hover && self->icone_hover.id != 0) ? self->icone_hover : self->icone;
    // Sem textura de hover, escurece levemente quando o mouse NÃO está em cima
    Color tint = (hover || self->icone_hover.id != 0) ? WHITE : (Color){ 215, 215, 215, 255 };
    Rectangle origem = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    DrawTexturePro(tex, origem, destino, (Vector2){ 0.0f, 0.0f }, 0.0f, tint);
}

void MinimapaDraw(MinimapaBody *self, ResourcesBody *resources, int screen_width, int screen_height) {
    DrawRectangle(0, 0, screen_width, screen_height, (Color){ 0, 0, 0, 200 });

    Rectangle destino = MinimapaDestinoMapa(screen_width, screen_height);
    float escala = destino.width / MINIMAPA_REF_W;

    if (self->base.id != 0) {
        Rectangle origem = { 0.0f, 0.0f, (float)self->base.width, (float)self->base.height };
        DrawTexturePro(self->base, origem, destino, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
    }

    float fonte = MINIMAPA_FONTE_REF * escala;
    for (int i = 0; i < MINIMAPA_TOTAL_SALAS; i++) {
        float t = self->alpha_alerta[i];   // 1 = pendente (amarelo), 0 = concluída (branco)

        // Nome centralizado
        Color cor = MinimapaMisturar(resources->branco, resources->amarelo, t);
        Vector2 dim = MeasureTextEx(resources->fonte_principal, MINIMAPA_NOMES[i], fonte, 1);
        Vector2 centro_nome = {
            destino.x + MINIMAPA_POS_NOME[i].x * escala,
            destino.y + MINIMAPA_POS_NOME[i].y * escala
        };
        DrawTextEx(resources->fonte_principal, MINIMAPA_NOMES[i],
                   (Vector2){ centro_nome.x - dim.x / 2.0f, centro_nome.y - dim.y / 2.0f },
                   fonte, 1, cor);

        // Ícone de alerta (some com fade). A Copa usa o ícone do café.
        bool eh_cafe = (i == MINIMAPA_SALA_COPA && self->icone_cafe.id != 0);
        Texture2D tex = eh_cafe ? self->icone_cafe : self->triangulo;
        if (tex.id != 0 && t > 0.01f) {
            float tw = (eh_cafe ? MINIMAPA_CAFE_W : MINIMAPA_TRI_W) * escala;
            float th = (eh_cafe ? MINIMAPA_CAFE_H : MINIMAPA_TRI_H) * escala;
            Rectangle dst = {
                destino.x + MINIMAPA_POS_TRIANGULO[i].x * escala - tw / 2.0f,
                destino.y + MINIMAPA_POS_TRIANGULO[i].y * escala - th / 2.0f,
                tw, th
            };
            Rectangle src = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
            DrawTexturePro(tex, src, dst, (Vector2){ 0.0f, 0.0f }, 0.0f, Fade(WHITE, t));
        }
    }

    const char *ajuda = "N, ESC OU CLIQUE NO ICONE: FECHAR";
    Vector2 ajuda_dim = MeasureTextEx(resources->fonte_principal, ajuda, 44.0f, 1);
    DrawTextEx(resources->fonte_principal, ajuda,
               (Vector2){ (screen_width - ajuda_dim.x) / 2.0f, screen_height * 0.93f },
               44.0f, 1, resources->lilas_claro);
}

void MinimapaUnload(MinimapaBody *self) {
    UnloadTexture(self->base);
    UnloadTexture(self->triangulo);
    UnloadTexture(self->icone_cafe);
    UnloadTexture(self->icone);
    UnloadTexture(self->icone_hover);
}