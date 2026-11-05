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
#include "include/database/database.h"

/** @brief Recursos globais do jogo. */
typedef struct ResourcesBody {
    Font fonte_principal; // Fonte VT323, usada em todas as cenas.
    GameData ranking[DATABASE_MAX_ENTRIES]; // Rank dos players com melhores scores
    int ranking_count; // Quantidade de dados que tem no ranking
}ResourcesBody;

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