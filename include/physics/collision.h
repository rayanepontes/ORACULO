/**
 * @file collision.h
 * @brief Sistema de detecção e resolução de colisões 2D.
 * @details Fornece as estruturas e funções para definir áreas de colisão,
 *          verificar interseções e resolver empurrões entre o player e o cenário.
 */

#ifndef COLLISION_H
#define COLLISION_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_COLISORES 600 // número máximo de retângulos de colisão por mapa

/** @struct CollisionRect
 *
 * @brief Retângulo de colisão com tag de identificação para debug.
 *
 * @param rect Área de colisão em coordenadas do mapa.
 * @param tag  Nome curto para identificar no modo debug (ex: "parede", "borda").
 */
typedef struct CollisionRect {
    Rectangle   rect;
    const char *tag;
} CollisionRect;

/**
 * @brief Verifica se dois retângulos se sobrepõem.
 *
 * @param a Primeiro retângulo.
 * @param b Segundo retângulo.
 * @return true se houver sobreposição, false caso contrário.
 */
bool CollisionCheck(Rectangle a, Rectangle b);

/**
 * @brief Resolve a colisão empurrando `mover` para fora de `fixo`.
 *
 * @details Calcula a menor sobreposição entre os dois retângulos e
 *          desloca `mover` para o lado que exige menos deslocamento.
 *          Deve ser chamada separadamente para X e Y.
 *
 * @param mover Ponteiro para o retângulo que se move — modificado in-place.
 * @param fixo  Retângulo estático (parede, borda).
 */
void CollisionResolve(Rectangle *mover, Rectangle fixo);

/**
 * @brief Desenha todas as hitboxes coloridas na tela (modo debug).
 *
 * @details Deve ser chamada dentro de BeginMode2D/EndMode2D para que as
 *          hitboxes apareçam nas coordenadas corretas do mapa.
 *
 * @param colisores Array de CollisionRect do mapa.
 * @param count     Quantidade de colisores no array.
 */
void CollisionDrawDebug(CollisionRect *colisores, int count);

#endif