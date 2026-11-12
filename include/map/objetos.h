/** @file objetos.h
 * @brief Módulo de objetos do mapa.
 * @details Gerencia os sprites dos objetos posicionados no mundo,
 *          separados do mapa base. Cada objeto tem posição, tamanho
 *          e visibilidade independentes.
 */
#ifndef OBJETOS_H
#define OBJETOS_H

#include <raylib.h>
#include <stdbool.h>
#include "map/map.h"

#define OBJETOS_PATH "assets/texture/mapa/objetos/"

// ─── Structs ──────────────────────────────────────────────────────────────────

/** @brief Representa um objeto visual posicionado no mundo. */
typedef struct ObjetoMapa {
    Texture2D sprite;
    Rectangle mundo_rect;  ///< Posição e tamanho no mundo (já com MAP_ESCALA)
    bool      visivel;
} ObjetoMapa;

/** @brief Estado completo de todos os objetos do mapa. */
typedef struct ObjetosMapaBody {
    // ── Minigame Alucinação ───────────────────────────────────────────────────
    ObjetoMapa alucinacao_pc_l;
    ObjetoMapa alucinacao_pc_r;

    // ── Recepção ──────────────────────────────────────────────────────────────
    ObjetoMapa recepcao_balcao;
    ObjetoMapa recepcao_mesa;
    ObjetoMapa recepcao_poltrona_l;
    ObjetoMapa recepcao_poltrona_r;
    ObjetoMapa recepcao_puff_1;
    ObjetoMapa recepcao_puff_2;
    ObjetoMapa recepcao_puff_3;
    ObjetoMapa recepcao_sofa;
    ObjetoMapa recepcao_vaso;
    ObjetoMapa mesa_de_centro;

    // ── Copa ──────────────────────────────────────────────────────────────────
    ObjetoMapa copa_balcao;
    ObjetoMapa copa_mesa;

    // ── Sala cofre ──────────────────────────────────────────────────────────────────
    ObjetoMapa estante;
    ObjetoMapa pc;

    // ── Elétrica ──────────────────────────────────────────────────────────────
    ObjetoMapa eletrica_1;
    ObjetoMapa eletrica_2;
    ObjetoMapa eletrica_esfregao;
    ObjetoMapa eletrica_maquina;
    ObjetoMapa eletrica_mini;

    // ── Triador ───────────────────────────────────────────────────────────────
    ObjetoMapa triador_capsula_1;
    ObjetoMapa triador_capsula_2;
    ObjetoMapa triador_esteira;
    ObjetoMapa triador_lixo;

    // ── Catraca (estados mutuamente exclusivos) ───────────────────────────────
    ObjetoMapa catraca_aberta;
    ObjetoMapa catraca_fechada;

    // ── Outros ────────────────────────────────────────────────────────────────
    ObjetoMapa rectangle_149;
} ObjetosMapaBody;

// ─── API pública ──────────────────────────────────────────────────────────────

/** @brief Carrega todos os sprites e define posições e visibilidade iniciais. */
void ObjetosMapaInit(ObjetosMapaBody *self);

/** @brief Desenha todos os objetos visíveis no mundo (dentro de BeginMode2D). */
void ObjetosMapaDraw(ObjetosMapaBody *self);

/** @brief Descarrega todos os sprites da memória. */
void ObjetosMapaUnload(ObjetosMapaBody *self);

#endif