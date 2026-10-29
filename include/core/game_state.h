/**
 * @file game_state.h
 * @brief Define os estados globais do jogo.
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

/** @enum GameState
 * @brief Estados possíveis do fluxo principal do jogo.
 *
 * @details A main usa esse enum para decidir qual cena atualizar e desenhar.
 */
typedef enum {
    STATE_MENU,      // Menu principal (inclui nome e ranking).
    STATE_PLAYING,   // Partida em andamento. 
    STATE_INTERFACE, // Quando uma determinada tela se sobrepõe a gameplay.
    STATE_GAME_OVER, // Tela de derrota.
    STATE_WIN,       // Tela de vitória.
    TOTAL_STATES     // Quantidade de estados.
} GameState;

#endif