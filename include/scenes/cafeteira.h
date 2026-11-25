/** @file cafeteira.h
 * @brief Cafeteira da copa: ao interagir, o player corre por alguns minutos.
 */
#ifndef CAFETEIRA_H
#define CAFETEIRA_H

#include <raylib.h>
#include <stdbool.h>
#include "core/interagivel.h"
#include "core/resources.h"
#include "entities/player.h"

/** @brief Duração do efeito, em segundos da partida (180 = 3 minutos). */
#define CAFETEIRA_DURACAO 180.0f

/** @brief Quanto tempo a mensagem de café fica na tela, em segundos. */
#define CAFETEIRA_AVISO_DURACAO 4.0f
/** @brief Parte final desse tempo em que a mensagem desvanece, em segundos. */
#define CAFETEIRA_AVISO_FADE    1.0f

/** @brief Estado da cafeteira. */
typedef struct CafeteiraBody {
    Interagivel objeto;   ///< Área no mapa que ativa a interação
    bool        usada;    ///< True depois que o café foi tomado
    bool        ativa;    ///< True enquanto o efeito está valendo
    float       tempo_fim;///< Valor de tempo_restante em que o efeito acaba
    float       aviso_timer; ///< Tempo restante da mensagem na tela (0 = invisível)
} CafeteiraBody;

void CafeteiraInit(CafeteiraBody *self);

/** @brief Toma o café: o player passa a correr até o tempo da partida chegar em tempo_fim. */
void CafeteiraAtivar(CafeteiraBody *self, PlayerBody *player, float tempo_restante);

/** @brief Encerra o efeito quando o tempo acabar. Chamar todo frame do gameplay. */
void CafeteiraUpdate(CafeteiraBody *self, PlayerBody *player, float tempo_restante);

/** @brief Mostra a mensagem de energia no centro superior da tela, desvanecendo no final. */
void CafeteiraDrawAviso(CafeteiraBody *self, ResourcesBody *resources,
                        int screen_width, int screen_height);

void CafeteiraDrawIndicador(CafeteiraBody *self, ResourcesBody *resources, Camera2D camera);

/** @brief Mostra o tempo restante do efeito logo abaixo do cronômetro. */
void CafeteiraDrawHUD(CafeteiraBody *self, ResourcesBody *resources,
                      float tempo_restante, int screen_width);

#endif