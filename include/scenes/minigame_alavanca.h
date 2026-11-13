/** @file minigame_alavanca.h
 * @brief Módulo do minigame Alavanca de Emergência.
 * @details O jogador consulta o manual para entender a situação, calcula o PIN
 *          (prioridade do refeitório + nível de emergência) e digita-o no painel
 *          para assumir o controle e executar a evacuação.
 */
#ifndef MINIGAME_ALAVANCA_H
#define MINIGAME_ALAVANCA_H

#include <raylib.h>
#include <stdbool.h>
#include "core/interagivel.h"
#include "core/resources.h"

// ─── Constantes ───────────────────────────────────────────────────────────────

/** @brief PIN correto: prioridade refeitório (60) + nível emergência (4). */
#define ALAVANCA_PIN_CORRETO   64
/** @brief Número de dígitos do PIN. */
#define ALAVANCA_PIN_DIGITOS   2
/** @brief Penalidade em segundos ao errar o PIN. */
#define ALAVANCA_PENALIDADE    15.0f
/** @brief Duração da animação de descida da alavanca antes de mostrar resultado. */
#define ALAVANCA_ANIM_TEMPO    2.0f

// ─── Enums ────────────────────────────────────────────────────────────────────

/** @brief Telas internas do minigame. */
typedef enum TelaAlavanca {
    TELA_ALAVANCA_PRINCIPAL,  ///< Painel com PIN + botões
    TELA_ALAVANCA_ERRO,       ///< Penalidade por PIN errado
    TELA_ALAVANCA_ACERTO,     ///< Vitória
} TelaAlavanca;

/** @brief Foco de navegação no painel. */
typedef enum FocoAlavanca {
    FOCO_ALAVANCA_PIN,     ///< Área de digitação + botão CONFIRMAR
    FOCO_ALAVANCA_EVACUAR, ///< Botão EVACUAR
} FocoAlavanca;

// ─── Structs ──────────────────────────────────────────────────────────────────

/** @brief Estado completo do minigame Alavanca de Emergência. */
typedef struct AlavancaBody {
    Texture2D instrucoes;           ///< minigame02.png
    bool      mostrando_instrucoes; ///< True até o jogador apertar ENTER
    TelaAlavanca tela_atual;
    FocoAlavanca foco;

    int  digitos[ALAVANCA_PIN_DIGITOS]; ///< Dígitos inseridos (0–9 cada)
    int  cursor_digito;                 ///< Qual dígito está sendo editado (0 ou 1)
    bool pin_confirmado;                ///< PIN foi confirmado com ENTER na área de PIN

    float anim_timer;   ///< Temporizador da animação da alavanca descendo
    bool  anim_rodando; ///< True enquanto a animação de 2s está acontecendo
    bool  anim_acertou; ///< True = alavanca desceu por acerto; False = por erro

    Texture2D alavanca_normal;    ///< alavanca_normal.png (fundo normal)
    Texture2D alavanca_puxada;    ///< alavanca_puxada.png
    Texture2D botao_confirmar_n;  ///< botao_confirmar_normal.png
    Texture2D botao_confirmar_s;  ///< botao_confirnar_slected.png
    Texture2D botao_evacuar_n;    ///< botao_evacuar_normal.png
    Texture2D botao_evacuar_s;    ///< botao_evacuar_selected.png

    Interagivel objeto; ///< Objeto no mapa que ativa o minigame

    bool concluido; ///< True quando o minigame foi concluído com sucesso
    bool cancelado; ///< True quando o jogador pressiona ESC para fechar
} AlavancaBody;

// ─── API pública ──────────────────────────────────────────────────────────────

/** @brief Inicializa o estado do minigame e carrega os assets. */
void AlavancaInit(AlavancaBody *self);

/** @brief Atualiza o estado do minigame a cada frame.
 *  @param self      Ponteiro para o AlavancaBody ativo.
 *  @param tempo_restante Ponteiro para o tempo restante da partida (penalidade desconta aqui).
 */
void AlavancaUpdate(AlavancaBody *self, float *tempo_restante);

/** @brief Desenha o indicador de interação no mundo (fora do minigame). */
void AlavancaDrawIndicador(AlavancaBody *self, ResourcesBody *resources, Camera2D camera);

/** @brief Desenha a tela completa do minigame. */
void AlavancaDraw(AlavancaBody *self, ResourcesBody *resources, int screen_width, int screen_height);

/** @brief Descarrega todos os assets do minigame. */
void AlavancaUnload(AlavancaBody *self);

#endif