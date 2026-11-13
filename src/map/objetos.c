/** @file objetos.c
 * @brief Implementação do módulo de objetos do mapa.
 */
#include "map/objetos.h"

// ─── Helper interno ───────────────────────────────────────────────────────────

/** @brief Cria um ObjetoMapa carregando o sprite e definindo posição/visibilidade.
 *
 * @param path     Caminho para o arquivo de textura.
 * @param x        Posição X no mundo (sem MAP_ESCALA).
 * @param y        Posição Y no mundo (sem MAP_ESCALA).
 * @param w        Largura no mundo (sem MAP_ESCALA). 0 = usa largura natural do sprite.
 * @param h        Altura no mundo (sem MAP_ESCALA). 0 = usa altura natural do sprite.
 * @param visivel  Visibilidade inicial.
 */
static ObjetoMapa ObjetoMapaCriar(const char *path,
                                   float x, float y,
                                   float w, float h,
                                   bool visivel) {
    ObjetoMapa obj;
    obj.sprite  = LoadTexture(path);
    float fw = (w > 0.0f) ? w : (float)obj.sprite.width;
    float fh = (h > 0.0f) ? h : (float)obj.sprite.height;
    obj.mundo_rect = (Rectangle){
        x * MAP_ESCALA,
        y * MAP_ESCALA,
        fw * MAP_ESCALA,
        fh * MAP_ESCALA
    };
    obj.visivel = visivel;
    return obj;
}

// ─── API pública ──────────────────────────────────────────────────────────────

void ObjetosMapaInit(ObjetosMapaBody *self) {
    // TODO: substituir os valores 0,0,0,0 pelas coordenadas reais de cada objeto
    // Passar x, y, w, h sem MAP_ESCALA (a função já multiplica)
    // w=0 e h=0 usam o tamanho natural do sprite

    // ── Minigame Alucinação ───────────────────────────────────────────────────
    self->alucinacao_pc_l = ObjetoMapaCriar(OBJETOS_PATH "alucinacao_pc_L.png", 1031, 436, 0, 0, true);
    self->alucinacao_pc_r = ObjetoMapaCriar(OBJETOS_PATH "alucinacao_pc_R.png", 1460, 434, 0, 0, true);

    // ── Recepção ──────────────────────────────────────────────────────────────
    self->recepcao_balcao     = ObjetoMapaCriar(OBJETOS_PATH "recepcao_balcao.png",     1525.32, 1157.73, 0, 0, true);
    self->recepcao_mesa       = ObjetoMapaCriar(OBJETOS_PATH "recepcao_mesa.png",       1315.59, 1450.38, 0, 0, true);
    self->recepcao_poltrona_l = ObjetoMapaCriar(OBJETOS_PATH "recepcao_poltrona_L.png", 1905, 1418, 0, 0, true);
    self->recepcao_poltrona_r = ObjetoMapaCriar(OBJETOS_PATH "recepcao_poltrona_R.png", 2258, 1427, 0, 0, true);
    self->mesa_de_centro      = ObjetoMapaCriar(OBJETOS_PATH "mesa_de_centro.png",      2017, 1520, 0, 0, true);
    self->recepcao_puff_1     = ObjetoMapaCriar(OBJETOS_PATH "recepcao_puff.png",       2253, 1396, 0, 0, true);
    self->recepcao_puff_2     = ObjetoMapaCriar(OBJETOS_PATH "recepcao_puff.png",       1404.78, 1517.28, 0, 0, true);
    self->recepcao_puff_3     = ObjetoMapaCriar(OBJETOS_PATH "recepcao_puff.png",       1252.03, 1426.97, 0, 0, true);
    self->recepcao_sofa       = ObjetoMapaCriar(OBJETOS_PATH "recepcao_sofa.png",       2007, 1340, 0, 0, true);
    self->recepcao_vaso       = ObjetoMapaCriar(OBJETOS_PATH "recepcao_vaso.png",       1941, 1328, 0, 0, true);

    // ── Sala ──────────────────────────────────────────────────────────────────
    self->estante        = ObjetoMapaCriar(OBJETOS_PATH "estante.png",        463, 702, 0, 0, true);
    self->pc             = ObjetoMapaCriar(OBJETOS_PATH "pc.png",             55, 635, 0, 0, true);

    // ── Copa ──────────────────────────────────────────────────────────────────
    self->copa_balcao    = ObjetoMapaCriar(OBJETOS_PATH "copa_balcao.png",    3417, 583, 0, 0, true);
    self->copa_mesa      = ObjetoMapaCriar(OBJETOS_PATH "copa_mesa.png",      3040, 775, 0, 0, true);

    // ── Elétrica ──────────────────────────────────────────────────────────────
    self->eletrica_1        = ObjetoMapaCriar(OBJETOS_PATH "eletrica.png",         2591, 1859, 0, 0, true);
    self->eletrica_2        = ObjetoMapaCriar(OBJETOS_PATH "eletrica.png",         2798, 1993, 0, 0, true);
    self->eletrica_esfregao = ObjetoMapaCriar(OBJETOS_PATH "eletrica_esfregao.png", 3384, 2002, 0, 0, true);
    self->eletrica_maquina  = ObjetoMapaCriar(OBJETOS_PATH "eletrica_maquina.png",  3306, 1899, 0, 0, true);
    self->eletrica_mini     = ObjetoMapaCriar(OBJETOS_PATH "eletrica_mini.png",     2944, 1890, 0, 0, true);

    // ── Triador ───────────────────────────────────────────────────────────────
    self->triador_capsula_1 = ObjetoMapaCriar(OBJETOS_PATH "triador_capsula.png", 291, 1845, 0, 0, true);
    self->triador_capsula_2 = ObjetoMapaCriar(OBJETOS_PATH "triador_capsula.png", 390, 1888, 0, 0, true);
    self->triador_esteira = ObjetoMapaCriar(OBJETOS_PATH "triador_esteira.png", 592, 1888, 0, 0, true);
    self->triador_lixo = ObjetoMapaCriar(OBJETOS_PATH "triador_lixo.png", 1103, 2007, 0, 0, true);

    // ── Catraca (fechada visível, aberta invisível até completar os minigames) ─
    self->catraca_fechada = ObjetoMapaCriar(OBJETOS_PATH "catraca_fechada.png", 2037, 1077, 0, 0, true);
    self->catraca_aberta  = ObjetoMapaCriar(OBJETOS_PATH "catraca_aberta.png",  2037, 1077, 0, 0, false);

    // ── Outros ────────────────────────────────────────────────────────────────
    self->rectangle_149 = ObjetoMapaCriar(OBJETOS_PATH "Rectangle 149.png", 1639, 1286, 0, 0, true);
}

void ObjetoMapaDrawSingle(ObjetoMapa *obj) {
    if (!obj->visivel) return;
    DrawTexturePro(
        obj->sprite,
        (Rectangle){ 0, 0, (float)obj->sprite.width, (float)obj->sprite.height },
        obj->mundo_rect,
        (Vector2){ 0, 0 }, 0.0f, WHITE
    );
}

void ObjetosMapaColetarVisiveis(ObjetosMapaBody *self, ObjetoMapa **out, int *count) {
    *count = 0;

#define ADD(obj) if ((obj).visivel && *count < OBJETOS_MAX_VISIVEIS) out[(*count)++] = &(obj)

    ADD(self->alucinacao_pc_l);
    ADD(self->alucinacao_pc_r);
    ADD(self->recepcao_balcao);
    ADD(self->recepcao_puff_3);
    ADD(self->recepcao_mesa);
    ADD(self->recepcao_puff_2);
    ADD(self->recepcao_vaso);
    ADD(self->recepcao_poltrona_l);
    ADD(self->recepcao_puff_1);
    ADD(self->recepcao_poltrona_r);
    ADD(self->recepcao_sofa);
    ADD(self->mesa_de_centro);
    ADD(self->copa_balcao);
    ADD(self->copa_mesa);
    ADD(self->estante);
    ADD(self->pc);
    ADD(self->eletrica_1);
    ADD(self->eletrica_2);
    ADD(self->eletrica_maquina);
    ADD(self->eletrica_esfregao);
    ADD(self->eletrica_mini);
    ADD(self->triador_capsula_1);
    ADD(self->triador_capsula_2);
    ADD(self->triador_esteira);
    ADD(self->triador_lixo);
    ADD(self->catraca_fechada);
    ADD(self->catraca_aberta);
    ADD(self->rectangle_149);

#undef ADD
}

void ObjetosMapaUnload(ObjetosMapaBody *self) {
    UnloadTexture(self->alucinacao_pc_l.sprite);
    UnloadTexture(self->alucinacao_pc_r.sprite);
    UnloadTexture(self->recepcao_balcao.sprite);
    UnloadTexture(self->recepcao_mesa.sprite);
    UnloadTexture(self->recepcao_poltrona_l.sprite);
    UnloadTexture(self->recepcao_poltrona_r.sprite);
    UnloadTexture(self->recepcao_puff_1.sprite);
    UnloadTexture(self->recepcao_puff_2.sprite);
    UnloadTexture(self->recepcao_puff_3.sprite);
    UnloadTexture(self->recepcao_sofa.sprite);
    UnloadTexture(self->recepcao_vaso.sprite);
    UnloadTexture(self->copa_balcao.sprite);
    UnloadTexture(self->copa_mesa.sprite);
    UnloadTexture(self->estante.sprite);
    UnloadTexture(self->mesa_de_centro.sprite);
    UnloadTexture(self->pc.sprite);
    UnloadTexture(self->eletrica_1.sprite);
    UnloadTexture(self->eletrica_2.sprite);
    UnloadTexture(self->eletrica_esfregao.sprite);
    UnloadTexture(self->eletrica_maquina.sprite);
    UnloadTexture(self->eletrica_mini.sprite);
    UnloadTexture(self->triador_capsula_1.sprite);
    UnloadTexture(self->triador_capsula_2.sprite);
    UnloadTexture(self->triador_esteira.sprite);
    UnloadTexture(self->catraca_fechada.sprite);
    UnloadTexture(self->catraca_aberta.sprite);
    UnloadTexture(self->rectangle_149.sprite);
}