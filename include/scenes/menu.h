/**
 * @file menu.h
 * @brief Cena de menu principal do jogo ORÁCULO.
 * @details Gerencia a navegação entre as subtelas do menu e
 *          dispara a transição para o gameplay.
 */

#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include "core/resources.h"

/** @enum TelaMenu
 *  
 * @brief Subtelas disponíveis dentro da cena de menu.
 * @param TELA_PRINCIPAL Tela inicial com as opções do menu.
 * @param TELA_NOME Tela inicial com as opções do menu.
 * @param TELA_RANKING Subtela de inserção de nome do jogador.
 * @param TELA_TOTAL Quantidade total de telas.
 */
typedef enum TelaMenu {
    TELA_PRINCIPAL, /**< Tela inicial com as opções do menu. */
    TELA_NOME,      /**< Subtela de inserção de nome do jogador. */
    TELA_RANKING,   /**< Subtela de ranking. */
    TELA_TOTAL      /**< Quantidade total de telas. */
} TelaMenu;

/** @enum MenuOpc
 * 
 * @brief Opções disponíveis no menu principal.
 * @param MENU_OPC_INICIAR  Inicia uma nova partida.
 * @param MENU_OPC_RANKING  Navega para a subtela de ranking.
 * @param MENU_OPC_OPC  Quantidade total de opções.
 */
typedef enum MenuOpc {
    MENU_OPC_INICIAR, /**< Inicia uma nova partida. */
    MENU_OPC_RANKING,     /**< Navega para a subtela de ranking. */
    MENU_TOTAL_OPC        /**< Quantidade total de opções. */
} MenuOpc;

/** @struct MenuBody
 * 
 * @brief Estado completo da cena de menu.
 * @param backgrounds[TELA_TOTAL] Uma textura de fundo por subtela.
 * @param tela_atual Subtela visível no momento.
 * @param opcao_atual Opção selecionada no menu principal.
 * @param confirmado True quando o jogador confirma com ENTER.
 */
typedef struct MenuBody {
    Texture2D backgrounds[TELA_TOTAL]; /**< Uma textura de fundo por subtela. */
    TelaMenu  tela_atual;              /**< Subtela visível no momento. */
    MenuOpc   opcao_atual;             /**< Opção selecionada no menu principal. */
    bool      confirmado;              /**< true quando o jogador confirma com ENTER. */
} MenuBody;

/** @brief Inicializa a cena de menu carregando todas as texturas.
 * @param self Ponteiro para o MenuBody a ser inicializado.
 */
void MenuInit(MenuBody *self);

/** @brief Processa o input e atualiza o estado do menu.
 * @param self Ponteiro para o MenuBody ativo.
 */
void MenuUpdate(MenuBody *self);

/** @brief Desenha a cena de menu na tela.
 * @param self      Ponteiro para o MenuBody ativo.
 * @param resources Ponteiro para os recursos globais (fonte, etc).
 * @param scream_width  Largura atual da tela em pixels.
 * @param scream_height Altura atual da tela em pixels.
 */
void MenuDraw(MenuBody *self, ResourcesBody *resources, int scream_width, int scream_height);

/** @brief Descarrega todos os recursos da cena de menu.
 * @param self Ponteiro para o MenuBody a ser descarregado.
 */
void MenuUnload(MenuBody *self);

#endif