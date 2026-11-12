/** @file minigame_triador.h
 * @brief Módulo do minigame Triador de Treinamento.
 * @details O jogador seleciona 6 colaboradores da lista (2 de cada setor)
 *          para compor a base de treinamento da IA de forma balanceada.
 */
#ifndef MINIGAME_TRIADOR_H
#define MINIGAME_TRIADOR_H

#include <raylib.h>
#include <stdbool.h>
#include "core/interagivel.h"
#include "core/resources.h"

// ─── Constantes ───────────────────────────────────────────────────────────────

#define TRIADOR_TOTAL_COLABORADORES 8
#define TRIADOR_MAX_SELECIONADOS    6
#define TRIADOR_TOTAL_SETORES       3
#define TRIADOR_POR_SETOR           2   ///< Quantidade correta de cada setor

// ─── Enums ────────────────────────────────────────────────────────────────────

/** @brief Telas internas do minigame. */
typedef enum TelaTriador {
    TELA_TRIADOR_PRINCIPAL,
    TELA_TRIADOR_ERRO,
    TELA_TRIADOR_ACERTO,
} TelaTriador;

typedef enum FocoTriador {
    FOCO_LISTA,
    FOCO_BOTAO,
} FocoTriador;

/** @brief Setores dos colaboradores. */
typedef enum SetorColaborador {
    SETOR_PESQUISA = 0,
    SETOR_TECNOLOGIA,
    SETOR_APOIO_OPERACOES,
} SetorColaborador;

// ─── Structs ──────────────────────────────────────────────────────────────────

/** @brief Dados de um colaborador disponível na lista. */
typedef struct Colaborador {
    const char      *nome;
    const char      *id;
    const char      *cargo;
    SetorColaborador setor;
    Texture2D        avatar;      ///< Imagem grande (lista esquerda)
    Texture2D        avatar_3x4;  ///< Imagem pequena (slot direita)
    bool             selecionado;
} Colaborador;

/** @brief Estado completo do minigame Triador de Treinamento. */
typedef struct TriadorBody {
    TelaTriador tela_atual;
    FocoTriador foco;

    Colaborador colaboradores[TRIADOR_TOTAL_COLABORADORES];
    int         cursor;                       ///< Índice do colaborador destacado na lista
    int         scroll_offset;               ///< Primeiro índice visível na lista
    int         total_selecionados;
    int         por_setor[TRIADOR_TOTAL_SETORES]; ///< Contagem por setor

    Texture2D   campo_vazio;    ///< Sprite de slot vazio na base
    Texture2D   background;     ///< Fundo da tela (triador_sem_contas.png)
    Texture2D   botao;          ///< Sprite do botão Treinar IA normal
    Texture2D   botao_selecionado; ///< Sprite do botão de treinar IA selecionado
    float aviso_timer;  ///< tempo restante do aviso de seleção incompleta (0 = invisível)

    Texture2D indicador;        ///< setinha amarela ao lado do card destacado
    float     indicador_timer;  ///< tempo acumulado para animação
    bool      indicador_offset; ///< alterna entre posição base e +5px

    Interagivel objeto;         ///< Objeto no mapa que ativa o minigame

    bool        concluido;
    bool        cancelado;
} TriadorBody;

// ─── API pública ──────────────────────────────────────────────────────────────

/** @brief Inicializa o estado do minigame e carrega os assets. */
void TriadorInit(TriadorBody *self);

/** @brief Atualiza o estado do minigame a cada frame. */
void TriadorUpdate(TriadorBody *self);

/** @brief Desenha o indicador de interação no mundo (fora do minigame). */
void TriadorDrawIndicador(TriadorBody *self, ResourcesBody *resources, Camera2D camera);

/** @brief Desenha a tela completa do minigame. */
void TriadorDraw(TriadorBody *self, ResourcesBody *resources, int screen_width, int screen_height);

/** @brief Descarrega todos os assets do minigame. */
void TriadorUnload(TriadorBody *self);

#endif