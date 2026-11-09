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

/** @brief Duração total de uma partida, em segundos (300 = 5 minutos). */
#define GAMEPLAY_TEMPO_TOTAL 300.0f
/** @brief Quantidade de páginas do manual (a primeira é a capa). */
#define MANUAL_TOTAL_PAGINAS 4

/** @enum TelaInterface
 *
 * @brief Subtelas de interface que podem se sobrepor ao gameplay.
 *
 * @details Quando o estado global é STATE_INTERFACE, esse enum determina
 *          qual overlay está visível e quais teclas são processadas.
 *          Adicione novos valores aqui à medida que novas telas forem criadas.
 *
 * @param TELA_VOLTAR_MENU Confirmação de saída para o menu principal.
 * @param TELA_INSTRUCOES  Instruções exibidas antes da partida começar.
 * @param TOTAL_TELAS      Quantidade total de telas de interface.
 */
typedef enum TelaInterface {
    TELA_VOLTAR_MENU, // Confirmação de saída para o menu principal.
    TELA_INSTRUCOES,  // Instruções exibidas antes da partida começar.
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
 * @param player            Entidade controlada pelo jogador.
 * @param map               Mapa atual do jogo.
 * @param camera            Câmera 2D que segue o player.
 * @param interface_atual   Subtela de interface ativa quando state == STATE_INTERFACE.
 * @param tempo_conclusao   Tempo de conclusão da partida em segundos.
 * @param tempo_restante    Segundos que faltam para o fim da partida (contagem regressiva).
 * @param cronometro_ativo  True enquanto o cronômetro está rodando (após as instruções).
 * @param instrucoes_vistas True depois que o jogador confirmou a tela de instruções.
 * @param manual_aberto   True enquanto o manual está sobreposto (o tempo continua).
 * @param manual_pagina   Índice da página atual do manual (0 = capa).
 * @param manual_paginas  Texturas das páginas do manual.
 * @param nickname          Nome do jogador copiado do menu (máx 6 chars + '\0').
 * @param terminou          True quando a partida chega ao fim (vitória ou derrota).
 * @param venceu            True se o jogador venceu; false se perdeu.
 * @param voltar_menu       True quando o jogador confirma que quer voltar ao menu.
 */
typedef struct GameplayBody {
    PlayerBody    player;
    MapBody       map;
    Camera2D      camera;
    TelaInterface interface_atual;   // subtela ativa quando state == STATE_INTERFACE
    int           tempo_conclusao;   // tempo de conclusão em segundos (preenchido ao vencer)
    float         tempo_restante;    // segundos restantes (regressivo)
    bool          cronometro_ativo;  // cronômetro rodando?
    bool          instrucoes_vistas; // jogador já passou pela tela de instruções?
    bool          manual_aberto;                           // manual na tela?
    int           manual_pagina;                           // página atual (0 = capa)
    Texture2D     manual_paginas[MANUAL_TOTAL_PAGINAS];    // PNGs do manual
    char          nickname[7];       // copiado do menu antes de iniciar
    bool          terminou;          // main lê: partida chegou ao fim
    bool          venceu;            // main lê: vitória (true) ou derrota (false)
    bool          voltar_menu;       // main lê: jogador pediu para voltar ao menu
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

/** @brief Encerra a partida com vitória e registra o tempo de conclusão.
 *
 * @details Para o cronômetro, define tempo_conclusao = tempo decorrido convertido para
 *          inteiro e marca terminou/venceu para a main trocar para STATE_WIN.
 *          Chame esta função quando o objetivo da fase for cumprido.
 *
 * @param self Ponteiro para o GameplayBody ativo.
 */
void GameplayVencer(GameplayBody *self);

/** @brief Descarrega todos os recursos da cena de gameplay.
 * @param self Ponteiro para o GameplayBody a ser descarregado.
 */
void GameplayUnload(GameplayBody *self);

#endif