/** @file minigame_reconector.h
 * @brief Minigame Reconector Neural — conecte todos os pares de módulos no grid.
 * @details O jogador clica num módulo e arrasta o mouse pelo grid criando cabos
 *          coloridos. O objetivo é conectar cada par de módulos da mesma cor.
 *          Passar por cima de um cabo existente apaga aquele cabo inteiro.
 *          Não é possível passar por cima de um módulo fixo de outro par.
 */
#ifndef MINIGAME_RECONECTOR_H
#define MINIGAME_RECONECTOR_H

#include <raylib.h>
#include <stdbool.h>
#include "core/interagivel.h"
#include "core/resources.h"

// ─── Constantes ───────────────────────────────────────────────────────────────

#define RECONECTOR_GRID_COLS   8
#define RECONECTOR_GRID_ROWS   8
#define RECONECTOR_TOTAL_PARES 8   ///< Total de pares de módulos no grid

/** @brief Valor de célula vazia (sem cabo nem âncora). */
#define RECONECTOR_VAZIO  -1

// ─── Enums ────────────────────────────────────────────────────────────────────

/** @brief Identificadores de cada par de módulos (mesma ordem do painel do design). */
typedef enum ParReconector {
    PAR_VISAO = 0,   ///< Azul     (visão computacional)
    PAR_VOZ,         ///< Verde    (reconhecimento de voz)
    PAR_LLM,         ///< Amarelo  (LLM / texto)
    PAR_FIREWALL,    ///< Vermelho (firewall ético)
    PAR_ML,          ///< Ciano    (machine learning)
    PAR_OPTICO,      ///< Roxo     (reconhecimento óptico)
    PAR_REDE,        ///< Laranja  (rede)
    PAR_TTS,         ///< Rosa     (text-to-speech)
} ParReconector;

/** @brief Telas internas do minigame. */
typedef enum TelaReconector {
    TELA_RECONECTOR_JOGO,
    TELA_RECONECTOR_ACERTO,
} TelaReconector;

// ─── Structs ──────────────────────────────────────────────────────────────────

/** @brief Uma célula do grid. */
typedef struct CelulaReconector {
    int  par;       ///< PAR_xxx que ocupa esta célula, ou RECONECTOR_VAZIO
    bool ancora;    ///< True se é um módulo fixo (não pode ser sobrescrito)
} CelulaReconector;

/** @brief Estado completo do minigame Reconector Neural. */
typedef struct ReconectorBody {
    Texture2D instrucoes;
    bool      mostrando_instrucoes;

    TelaReconector tela_atual;

    /** @brief Grid 8×8: grid[col][linha] */
    CelulaReconector grid[RECONECTOR_GRID_COLS][RECONECTOR_GRID_ROWS];

    /** @brief Quantas células cada par já conectou (tamanho do cabo). */
    int  cabo_tamanho[RECONECTOR_TOTAL_PARES];

    /** @brief Caminho de cada cabo, na ordem: caminho_xxx[par][0..cabo_tamanho[par]-1]. */
    int caminho_col[RECONECTOR_TOTAL_PARES][RECONECTOR_GRID_COLS * RECONECTOR_GRID_ROWS];
    int caminho_lin[RECONECTOR_TOTAL_PARES][RECONECTOR_GRID_COLS * RECONECTOR_GRID_ROWS];

    /** @brief True quando os dois extremos do par estão conectados. */
    bool par_concluido[RECONECTOR_TOTAL_PARES];

    /** @brief Par que está sendo arrastado agora (-1 = nenhum). */
    int  arrastando_par;

    /** @brief Coluna/linha da última célula do cabo em construção. */
    int  arrastar_col;
    int  arrastar_lin;

    int  origem_col;   ///< Âncora de onde o cabo atual começou
    int  origem_lin;

    /** @brief Cor de cada par (preenchida em Init). */
    Color cores[RECONECTOR_TOTAL_PARES];

    /** @brief Fundo do minigame. */
    Texture2D background;

    /** @brief Botão resetar (normal e selected). */
    Texture2D botao_resetar;
    Texture2D botao_resetar_sel;
    Rectangle rect_botao_resetar;
    bool      hover_botao_resetar;

    Interagivel objeto;  ///< Objeto no mapa que ativa o minigame
    bool concluido;
    bool cancelado;
} ReconectorBody;

// ─── API pública ──────────────────────────────────────────────────────────────

void ReconectorInit(ReconectorBody *self);
void ReconectorUpdate(ReconectorBody *self, ResourcesBody *resources);
void ReconectorDrawIndicador(ReconectorBody *self, ResourcesBody *resources, Camera2D camera);
void ReconectorDraw(ReconectorBody *self, ResourcesBody *resources, int sw, int sh);
void ReconectorUnload(ReconectorBody *self);

#endif