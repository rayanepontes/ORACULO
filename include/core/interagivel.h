/**
 * @file interagivel.h
 * @brief Utilitário para objetos interativos posicionados no mundo.
 * @details Encapsula a posição, o tamanho e a área de interação de qualquer
 *          objeto com o qual o player possa interagir por proximidade.
 */

#ifndef INTERAGIVEL_H
#define INTERAGIVEL_H

#include <raylib.h>
#include <stdbool.h>

/** @struct Interagivel
 * @brief Representa um objeto interativo no mundo do jogo.
 *
 * @param mundo_rect     Posição e tamanho do objeto no mundo (para desenhar e colidir).
 * @param interacao_rect Área expandida ao redor do objeto onde o player pode interagir.
 * @param ativo          False quando o objeto já foi usado e não aceita mais interação.
 */
typedef struct Interagivel {
    Rectangle mundo_rect;
    Rectangle interacao_rect;
    bool      ativo;
} Interagivel;

/** @brief Cria um Interagivel calculando automaticamente a área de interação.
 * @param x      Posição x no mundo (já com escala).
 * @param y      Posição y no mundo (já com escala).
 * @param w      Largura do objeto no mundo.
 * @param h      Altura do objeto no mundo.
 * @param margem Expansão em pixels ao redor do objeto para a área de interação.
 * @return Interagivel inicializado com ativo = true.
 */
Interagivel InteragivelCriar(float x, float y, float w, float h, float margem);

/** @brief Verifica se um ponto está dentro da área de interação.
 * @param self  Ponteiro para o Interagivel.
 * @param ponto Posição do player no mundo (use o centro do sprite).
 * @return True se o ponto estiver na área e o objeto estiver ativo.
 */
bool InteragivelPoderInteragir(Interagivel *self, Vector2 ponto);

#endif