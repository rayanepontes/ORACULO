/**
 * @file gameplay.h
 * @brief Cena de gameplay do jogo ORÁCULO.
 * @details Gerencia o loop de partida: movimentação do player, câmera,
 *          mapa e as telas de interface que se sobrepõem ao jogo (pausa,
 *          confirmação de saída, etc.).
 */

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <raylib.h>
#include <stdbool.h>
#include "core/game_state.h"
#include "core/resources.h"
#include "entities/player.h"
#include "map/map.h"

/** @enum TelaInterface
 *
 * @brief Subtelas de interface que podem se sobrepor ao gameplay.
 *
 * @details Quando o estado global é STATE_INTERFACE, esse enum determina
 *          qual overlay está visível e quais teclas são processadas.
 *          Adicione novos valores aqui à medida que novas telas forem criadas.
 *
 * @param TELA_VOLTAR_MENU Confirmação de saída para o menu principal.
 * @param TOTAL_TELAS      Quantidade total de telas de interface.
 */
typedef enum TelaInterface {
    TELA_VOLTAR_MENU, // Confirmação de saída para o menu principal.
    TOTAL_TELAS       // Quantidade total de telas de interface.
} TelaInterface;

typedef enum OpcVoltarMenu{
    VOLTAR_MENU,
    VOLTAR_JOGO,
    TOTAL_OPC
}OpcVoltarMenu;

/** @struct GameplayBody
 *
 * @brief Estado completo da cena de gameplay.
 *
 * @details Agrupa o player, o mapa, a câmera e todos os flags que a main
 *          precisa ler ao final de cada frame para decidir transições de estado.
 *
 * @param player          Entidade controlada pelo jogador.
 * @param map             Mapa atual do jogo.
 * @param camera          Câmera 2D que segue o player.
 * @param interface_atual Subtela de interface ativa quando state == STATE_INTERFACE.
 * @param score           Pontuação da partida — reservado para o sistema de tempo.
 * @param nickname        Nome do jogador copiado do menu (máx 6 chars + '\0').
 * @param terminou        True quando a partida chega ao fim (vitória ou derrota).
 * @param venceu          True se o jogador venceu; false se perdeu.
 * @param voltar_menu     True quando o jogador confirma que quer voltar ao menu.
 */
typedef struct GameplayBody {
    PlayerBody    player;
    MapBody       map;
    Camera2D      camera;
    TelaInterface interface_atual; // subtela ativa quando state == STATE_INTERFACE
    int           score;           // reservado — implementar com o sistema de tempo
    char          nickname[7];     // copiado do menu antes de iniciar
    bool          terminou;        // main lê: partida chegou ao fim
    bool          venceu;          // main lê: vitória (true) ou derrota (false)
    bool          voltar_menu;     // main lê: jogador pediu para voltar ao menu
    OpcVoltarMenu opc_confirmacao;
} GameplayBody;

/** @brief Inicializa a cena de gameplay carregando player, mapa e câmera.
 * @param self Ponteiro para o GameplayBody a ser inicializado.
 */
void GameplayInit  (GameplayBody *self);

/** @brief Processa o input e atualiza o estado da partida.
 *
 * @details Quando state == STATE_INTERFACE, processa apenas as teclas da
 *          tela de interface ativa e trava o restante do update. Caso
 *          contrário, atualiza o player e a câmera normalmente.
 *
 * @param self  Ponteiro para o GameplayBody ativo.
 * @param state Ponteiro para o estado global — pode ser alterado internamente.
 */
void GameplayUpdate(GameplayBody *self, GameState *state);

/** @brief Desenha a partida e, se aplicável, a tela de interface sobreposta.
 *
 * @details O mapa e o player são sempre desenhados dentro de BeginMode2D/EndMode2D.
 *          Quando state == STATE_INTERFACE, o overlay correspondente é desenhado
 *          fora do modo 2D (em coordenadas de tela).
 *
 * @param self          Ponteiro para o GameplayBody ativo.
 * @param state         Ponteiro para o estado global (leitura).
 * @param resources     Ponteiro para os recursos globais (fonte, cores).
 * @param scream_width  Largura atual da tela em pixels.
 * @param scream_height Altura atual da tela em pixels.
 */
void GameplayDraw  (GameplayBody *self, GameState *state, ResourcesBody *resources, int scream_width, int scream_height);

/** @brief Descarrega todos os recursos da cena de gameplay.
 * @param self Ponteiro para o GameplayBody a ser descarregado.
 */
void GameplayUnload(GameplayBody *self);

#endif