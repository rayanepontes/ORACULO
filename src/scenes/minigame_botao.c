/** @file minigame_botao.c
 * @brief Implementação do botão do alarme.
 */
#include "scenes/minigame_botao.h"
#include "map/map.h"

#define BOTAO_PATH "assets/texture/minigame_botao/"

// ─── Layout (canto superior esquerdo de cada PNG, no design de 1920x1080) ─────
#define REF_W 1920.0f
#define REF_H 1080.0f

#define POS_BOTAO_X        302.74f
#define POS_BOTAO_Y        112.18f
#define POS_BARRA_X       1615.92f
#define POS_BARRA_Y        138.21f
#define POS_PORCENT_X     1633.45f   // preenchimento fica dentro da moldura da barra
#define POS_PORCENT_Y      151.35f
#define POS_TEXTO_CX      1681.0f   // centro horizontal do texto (alinhado ao ícone do E)
#define POS_TEXTO_Y       1000.8f
#define TAMANHO_TEXTO       30.0f

void BotaoInit(BotaoBody *self) {
    self->fundo             = LoadTexture(BOTAO_PATH "fundo.png");
    self->botao_normal      = LoadTexture(BOTAO_PATH "botao.png");
    self->botao_pressionado = LoadTexture(BOTAO_PATH "botao_pressed.png");
    self->barra_normal      = LoadTexture(BOTAO_PATH "barra.png");
    self->barra_pressionada = LoadTexture(BOTAO_PATH "barra_E_pressed.png");
    self->barra_porcentagem = LoadTexture(BOTAO_PATH "barra_porcentagem.png");

    // TODO: ajuste para a posição real do botão no mapa (x1, y1, x2, y2 em pixels do mapa)
    const float x1 = 2545, y1 = 1233, x2 = 2680, y2 = 1415;
    self->objeto = InteragivelCriar(x1 * MAP_ESCALA, y1 * MAP_ESCALA,
                                    (x2 - x1) * MAP_ESCALA, (y2 - y1) * MAP_ESCALA, 40.0f);
    BotaoResetar(self);
}

void BotaoResetar(BotaoBody *self) {
    self->progresso = 0.0f;
    self->segurando = false;
    self->concluido = false;
    self->cancelado = false;
}
    
void BotaoUpdate(BotaoBody *self) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        self->cancelado = true;
        return;
    }

    float dt = GetFrameTime();
    self->segurando = IsKeyDown(KEY_E);

    if (self->segurando) {
        self->progresso += dt / BOTAO_TEMPO_SEGURAR;
    } else {
        self->progresso -= (dt / BOTAO_TEMPO_SEGURAR) * BOTAO_VELOCIDADE_RECUO;
    }

    if (self->progresso < 0.0f) self->progresso = 0.0f;
    if (self->progresso >= 1.0f) {
        self->progresso = 1.0f;
        self->concluido = true;
    }
}

// Deslocamento do texto em relação ao centro-topo do objeto (em pixels do mundo)
#define BOTAO_INDICADOR_OFFSET_X   -55.0f   // positivo = direita, negativo = esquerda
#define BOTAO_INDICADOR_OFFSET_Y  35.0f   // maior = mais alto, menor = mais baixo

void BotaoDrawIndicador(BotaoBody *self, ResourcesBody *resources, Camera2D camera) {
    if (!self->objeto.ativo) return;

    Vector2 pos_tela = GetWorldToScreen2D(
        (Vector2){ self->objeto.mundo_rect.x + self->objeto.mundo_rect.width / 2.0f + BOTAO_INDICADOR_OFFSET_X,
                   self->objeto.mundo_rect.y - BOTAO_INDICADOR_OFFSET_Y },
        camera);

    const char *texto   = "E - interagir";
    float       tamanho = 36.0f;
    Vector2     dim     = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);
    bool        visivel = ((int)(GetTime() * 2) % 2 == 0);
    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ pos_tela.x - dim.x / 2.0f, pos_tela.y },
               tamanho, 1, visivel ? resources->amarelo : resources->lilas_claro);
}

/** @brief Desenha uma textura na posição de referência (1920x1080), escalada para a tela. */
static void BotaoDrawRef(Texture2D tex, float x, float y, float sx, float sy) {
    if (tex.id == 0) return;
    DrawTexturePro(tex,
                   (Rectangle){ 0, 0, (float)tex.width, (float)tex.height },
                   (Rectangle){ x * sx, y * sy, tex.width * sx, tex.height * sy },
                   (Vector2){ 0, 0 }, 0.0f, WHITE);
}

void BotaoDraw(BotaoBody *self, ResourcesBody *resources, int screen_width, int screen_height) {
    float sx = screen_width  / REF_W;
    float sy = screen_height / REF_H;

    // Fundo ocupa a tela inteira
    DrawTexturePro(self->fundo,
                   (Rectangle){ 0, 0, (float)self->fundo.width, (float)self->fundo.height },
                   (Rectangle){ 0, 0, (float)screen_width, (float)screen_height },
                   (Vector2){ 0, 0 }, 0.0f, WHITE);

    // Botão (normal ou pressionado)
    BotaoDrawRef(self->segurando ? self->botao_pressionado : self->botao_normal,
                 POS_BOTAO_X, POS_BOTAO_Y, sx, sy);

    // Barra (normal ou com o E pressionado)
    BotaoDrawRef(self->segurando ? self->barra_pressionada : self->barra_normal,
                 POS_BARRA_X, POS_BARRA_Y, sx, sy);

    // Preenchimento: revelado de baixo pra cima conforme o progresso
    Texture2D p = self->barra_porcentagem;
    if (p.id != 0 && self->progresso > 0.0f) {
        float altura_visivel = p.height * self->progresso;
        float corte_topo     = p.height - altura_visivel;   // parte de cima que ainda está escondida

        DrawTexturePro(p,
                       (Rectangle){ 0, corte_topo, (float)p.width, altura_visivel },
                       (Rectangle){ POS_PORCENT_X * sx,
                                    (POS_PORCENT_Y + corte_topo) * sy,
                                    p.width * sx, altura_visivel * sy },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
    }

    // Texto piscando
    const char *texto = "PRESSIONE \"E\" PARA INTERAGIR";
    float tamanho = TAMANHO_TEXTO * sy;
    Vector2 dim = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);
    bool visivel = ((int)(GetTime() * 2) % 2 == 0);
    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ POS_TEXTO_CX * sx - dim.x / 2.0f, POS_TEXTO_Y * sy },
               tamanho, 1, visivel ? resources->amarelo : resources->branco);
}

void BotaoUnload(BotaoBody *self) {
    UnloadTexture(self->fundo);
    UnloadTexture(self->botao_normal);
    UnloadTexture(self->botao_pressionado);
    UnloadTexture(self->barra_normal);
    UnloadTexture(self->barra_pressionada);
    UnloadTexture(self->barra_porcentagem);
}