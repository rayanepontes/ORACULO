#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <raylib.h>
#include <stdbool.h>
#include "core/game_state.h"
#include "entities/player.h"
#include "map/map.h"

typedef struct GameplayBody{
    // mapa, jogador, inimigos: os tipos que o time já tem
    PlayerBody player;
    MapBody    map;
    Camera2D camera;
    int        score;       // reservado — implementar com o sistema de tempo
    char       nickname[7]; // copiado do menu antes de iniciar
    bool       terminou;    // main lê: partida chegou ao fim
    bool       venceu;      // main lê: vitória (true) ou derrota (false)
    bool       voltar_menu; // main lê: jogador pediu para voltar ao menu
} GameplayBody;

void GameplayInit  (GameplayBody *self);
void GameplayUpdate(GameplayBody *self, GameState *state);
void GameplayDraw  (GameplayBody *self);
void GameplayUnload(GameplayBody *self);

#endif