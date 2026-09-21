/** @file player.h
 * @brief Entidade player.
 * @details Aqui é onde fica o gerenciamento das ações do player e de
 *          suas respectivas animações.
 */
#ifndef PLAYER_H
#define PLAYER_H   

#include <raylib.h>
#include "graphics/animation.h"

/** @enum PlayerDirection
 * 
 * @brief Define o lado para qual o player está se movimentando.
 * 
 * @details É apenas uma maneira de deixar o código mais legivel. Fica muito
 *          melhor ler e entender esquerda ou direita doque -1 e 1.
 */
typedef enum PlayerDirection {
    LEFT  = -1,
    RIGHT =  1,
} PlayerDirection;

/** @enum PlayerActionType
 * 
 * @brief Facilita a definição do tipo de animação que irá ser usada em determinado momento.
 * 
 * @details É apenas uma maneira de deixar o código mais legivel. Fica muito
 *          melhor ler e entender que no array de animações você está mexendo 
 *          na parte que fica o idle do que no indice 0 por exemplo.
 */
typedef enum PlayerActionType{
    idle,
    walk,
    run,
    atk,
    hurt,
    death,
    total_actions
} PlayerActionType;

/** @struct PlayerBody
 * 
 * @brief Estrutura que armazena o estado de um player.
 * 
 * @details Essa estrutura age como se fosse uma classe, guardando
 *          todos os atributos necessários para gerenciar o player.
 * 
 * @param position Coordenadas (x, y) onde o player está localizado.
 * @param direction Lado para qual o player está virado.
 * @param action_atual Guarda qual é o tipo de ação que o player está executando no momento.
 * @param textures Array de texturas que um player possui.
 * @param anims Array de animações que um player possui.
 */
typedef struct PlayerBody{
    Vector2 position; //Coordenadas (x, y) onde o player está localizado.
    PlayerDirection direction; //Lado para qual o player está virado.
    PlayerActionType action_atual; //Guarda qual é o tipo de ação que o player está executando no momento.
    Texture2D textures[total_actions]; //Array de texturas que um player possui.
    AnimationBody anims[total_actions]; //Array de animações que um player possui.
} PlayerBody;


/** @brief Carrega texturas e inicializa posição do player.
 * 
 * @details Essa função usa uma outra função privada de player.c para carregar todas as texturas e corpos de animação
 *          que o player possui. Em seguida dita a posição, direção e animação iniciais do player.
 * 
 * @param self Ponteiro para uma entidade player.
 */
void PlayerInit(PlayerBody *self);

/** @brief Reseta o estado do player.
 * 
 * @details Essa função usa uma outra função privada de player.c para resetar o quadro de
 *          todas as animações do player para o estado inicial delas. Em seguida reseta a 
 *          posição, direção e animação do player para o estado incial.
 * 
 * @param self Ponteiro para uma entidade player.
 */
void PLayerReset(PlayerBody *self);

/** @brief Atualiza as ações do player.
 * 
 * @details Por enquanto essa função só atualiza o player em relação a sua movimentação e que tipo de
 *          animação será gerada a depender da ação que está acontecendo.
 * 
 * @param self Ponteiro para uma entidade player.
 */
void PlayerUpdateAction(PlayerBody *self);

/** @brief Desenha o player na tela.
 * 
 * @details Essa função usa as funções de animation.h para recortar a frame da animação
 *          e depois desenha esse recorte na tela.
 * 
 * @param self Ponteiro para uma entidade player.
 */
void PlayerDraw(PlayerBody *self);

/** @brief Descarrega as texturas do player.
 * 
 * @details Essa função usa um loop e vai descarregando todas as texturas do player do buffer.
 * 
 * @param self Ponteiro para uma entidade player.
 */
void PlayerUnload(PlayerBody *self);

#endif