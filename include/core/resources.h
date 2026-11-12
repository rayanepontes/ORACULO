/**
 * @file resources.h
 * @brief Recursos globais compartilhados por todas as cenas do jogo.
 * @details Centraliza informações que precisam estar disponíveis durante toda
 *          a execução. Deve ser inicializado uma única vez
 *          no main e descarregado ao encerrar o jogo.
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#include <raylib.h>
#include "database/database.h"

#define RESOURCES_FONTE_PATH "assets/fonts/VT323/VT323-Regular.ttf"
#define RESOURCES_FONTE_SIZE 64

/**
 * @struct ResourcesBody
 * @brief Recursos globais compartilhados por todas as cenas.
 * @param fonte_principal Fonte VT323, usada em todas as cenas.
 * @param ranking         Entradas do ranking carregadas do arquivo.
 * @param ranking_count   Número de entradas válidas em ranking.
 * @param amarelo         Cor amarela da paleta do jogo.
 * @param branco          Branco puro.
 * @param lilas_escuro    Lilás escuro da paleta.
 * @param lilas_claro     Lilás claro da paleta.
 * @param ciano_claro     Ciano clarinho.
 * @param letra_caixa     Cor das letras na caixa de nome.
 */
typedef struct ResourcesBody {
    Font     fonte_principal;
    GameData ranking[DATABASE_MAX_ENTRIES];
    int      ranking_count;
    Color    amarelo;
    Color    branco;
    Color    azul_escuro;
    Color    lilas_escuro;
    Color    lilas_claro;
    Color    ciano_claro; 
    Color    letra_caixa;
} ResourcesBody;

/**
 * @brief Inicializa e carrega todos os recursos globais.
 * @param self Ponteiro para o ResourcesBody a ser inicializado.
 */
void ResourcesInit(ResourcesBody *self);

/**
 * @brief Descarrega todos os recursos globais da memória.
 * @param self Ponteiro para o ResourcesBody a ser descarregado.
 */
void ResourcesUnload(ResourcesBody *self);

#endif