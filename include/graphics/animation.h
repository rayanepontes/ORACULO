/** @file animation.h
 * @brief Sistema de controle e corte de frames para spritesheets.
 * @details Gerencia a contagem de tempo, transição de quadros e cálculo de retângulos
 *          de origem para renderização com a Raylib.
 */
#ifndef ANIMATION_H  
#define ANIMATION_H

#include <raylib.h>

/** @enum AnimationType
 * 
 * @brief Define o comportamento do ciclo de vida da animação
 * 
 * @note Animações do tipo ONESHOT devem ser resetadas manualmente caso precise
 *       reproduzi-las novamente.
 */
typedef enum AnimationType {
    REPEATING = 1, //Animações que ficam em loop contínuo.
    ONESHOT   = 2, //Animações que rodam uma única vez.
} AnimationType;


/** @struct AnimationBody
 * 
 * @brief Estrutura que armazena o estado de uma animação 2D.
 * 
 * @details Essa estrutura age como se fosse uma classe, guardando
 *          todos os atributos necessários para animar uma imagem.
 * 
 * @param first_idx Indice do primeiro quadro.
 * @param last_idx Indice do último quadro.
 * @param frame_atual_idx Indice do quadro atual.
 * @param step Distância que saltamos ao passar para o próximo quadro.
 * @param speed Velocidade da animação do quadro.
 * @param duration_left Duração restante para mudar para o próximo quadro.
 * @param frames_por_linha Quantidade de quadros em cada linha do spritesheet.
 * @param spr_dimension Dimensão de cada quadro individual em pixels (ex: 16.0f).
 * @param type Tipo de animação (repeating ou oneshot).
 */
typedef struct AnimationBody{
    int first_idx; //Indice do primeiro quadro.
    int last_idx; //Indice do último quadro.
    int frame_atual_idx; //Indice do quadro atual.
    int step; //Distância que saltamos ao passar para o próximo quadro.

    float speed; //Velocidade da animação do quadro.
    float duration_left; //Duração restante para mudar para o próximo quadro.

    int frames_por_linha;  //Quantidade de quadros em cada linha do spritesheet.
    float spr_dimension;    //Dimensão de cada quadro individual em pixels (ex: 16.0f).

    AnimationType type; //Tipo de animação (repeating ou oneshot).
} AnimationBody;


/** @brief Atualiza a animação quando dentro de um loop.
 * 
 * @details Essa função subtrai a variação de tempo (delta_time) do quadro anterior para o atual da duração restante, até que
 * não haja mais duração, momento em que você incrementa o indice até chegar no limite, então 
 * decide oque fazer ao chegar no fim da animação.
 * 
 * @param self Ponteiro para uma estrutura de animação.
 */
void AnimationUpdate(AnimationBody *self);


/** @brief Calcula o retângulo de corte para o quadro atual da animação.
 * 
 * @details Esta função pega o índice do quadro atual e converte em coordenadas 2D (x, y)
 * dentro da imagem do spritesheet.
 * 
 * @param self Ponteiro para uma estrutura de animação.
 * 
 * @return Rectangle Estrutura da Raylib contendo a região a ser desenhada.
 */
Rectangle AnimationFrame(AnimationBody *self); 

#endif