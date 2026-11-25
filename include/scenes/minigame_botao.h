/** @file minigame_botao.h
 * @brief Botão de desligar o alarme: o jogador segura E até a barra completar.
 */
#ifndef MINIGAME_BOTAO_H
#define MINIGAME_BOTAO_H

#include <raylib.h>
#include <stdbool.h>
#include "core/interagivel.h"
#include "core/resources.h"

/** @brief Segundos segurando E para completar a barra. */
#define BOTAO_TEMPO_SEGURAR    3.0f
/** @brief Quão mais rápido a barra esvazia ao soltar E (1.0 = mesma velocidade de encher). */
#define BOTAO_VELOCIDADE_RECUO 2.0f

/** @brief Estado do botão do alarme. */
typedef struct BotaoBody {
    float       progresso;  ///< 0.0 a 1.0
    bool        segurando;  ///< True enquanto E está pressionado
    bool        concluido;
    bool        cancelado;
    Interagivel objeto;

    Texture2D fundo;              ///< fundo.png
    Texture2D botao_normal;       ///< botao.png
    Texture2D botao_pressionado;  ///< botao_pressed.png
    Texture2D barra_normal;       ///< barra.png
    Texture2D barra_pressionada;  ///< barra_E_pressed.png
    Texture2D barra_porcentagem;  ///< barra_porcentagem.png (preenchimento)
} BotaoBody;

void BotaoInit(BotaoBody *self);      // carrega assets e cria o objeto (chamar UMA vez)
void BotaoResetar(BotaoBody *self);   // zera só o estado (chamar a cada abertura)
void BotaoUnload(BotaoBody *self);
void BotaoUpdate(BotaoBody *self);
void BotaoDrawIndicador(BotaoBody *self, ResourcesBody *resources, Camera2D camera);
void BotaoDraw(BotaoBody *self, ResourcesBody *resources, int screen_width, int screen_height);

#endif