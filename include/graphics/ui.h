/**
 * @file ui.h
 * @brief Componentes de interface de usuário reutilizáveis.
 * @details Funções genéricas para renderizar e processar input de elementos
 *          de UI (seletores de opções, etc.) que se repetem em múltiplas cenas.
 */

#ifndef UI_H
#define UI_H

#include <raylib.h>

/** @brief Retorna true se UP ou W foi pressionado neste frame. */
static inline bool InputCima(void)    { return IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W); }
/** @brief Retorna true se DOWN ou S foi pressionado neste frame. */
static inline bool InputBaixo(void)   { return IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S); }
/** @brief Retorna true se LEFT ou A foi pressionado neste frame. */
static inline bool InputEsquerda(void){ return IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A); }
/** @brief Retorna true se RIGHT ou D foi pressionado neste frame. */
static inline bool InputDireita(void) { return IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D); }

/** @struct UiOpcaoEstilo
 *
 * @brief Define a aparência visual de um seletor de opções.
 *
 * @param size_normal   Tamanho da fonte para opções não selecionadas.
 * @param size_select   Tamanho da fonte para a opção selecionada.
 * @param cor_normal    Cor do texto para opções não selecionadas.
 * @param cor_select    Cor do texto para a opção selecionada.
 * @param espacamento   Distância vertical em pixels entre cada opção.
 */
typedef struct UiOpcaoEstilo {
    float size_normal;
    float size_select;
    Color cor_normal;
    Color cor_select;
    float espacamento;
} UiOpcaoEstilo;

/**
 * @brief Calcula os retângulos de cada opção com a mesma fórmula usada pelo Draw.
 * @details Usado para detecção de hover e clique do mouse.
 *
 * @param fonte         Fonte usada na renderização.
 * @param labels        Array de strings com os textos das opções.
 * @param total         Quantidade de opções.
 * @param estilo        Tamanhos e espaçamento do seletor.
 * @param pos_y_inicial Posição Y do primeiro item na tela.
 * @param scream_width  Largura da tela em pixels.
 * @param rects_out     Array de Rectangle a ser preenchido (deve ter `total` entradas).
 */
void UiOpcaoCalcRects(Font fonte, const char **labels, int total,
                      UiOpcaoEstilo estilo, float pos_y_inicial,
                      int scream_width, Rectangle *rects_out);

/**
 * @brief Processa o input de navegação de um seletor de opções.
 *
 * @details Escuta KEY_UP e KEY_DOWN para mover o índice (com wrap circular)
 *          e KEY_ENTER para confirmar. Teclas específicas de cada cena
 *          (como KEY_ESCAPE) devem ser tratadas pelo código chamador.
 *
 * @param opcao_atual  Ponteiro para o índice atual — atualizado internamente.
 * @param total        Quantidade total de opções.
 * @param som          Ponteiro para o Sound tocado ao trocar de opção, ou NULL para silêncio.
 * @param som_confirm  Ponteiro para o Sound tocado ao confirmar (ENTER), ou NULL para silêncio.
 * @param rects        Array de Rectangle das opções (de UiOpcaoCalcRects), ou NULL.
 * @return             Índice da opção confirmada com ENTER, ou -1 se nada foi confirmado.
 */
int UiOpcaoUpdate(int *opcao_atual, int total, Sound *som, Sound *som_confirm, Rectangle *rects);

/**
 * @brief Desenha uma lista de opções navegável, centralizada horizontalmente.
 *
 * @details Percorre o array de labels e desenha cada um com o estilo
 *          correspondente (selecionado ou normal). O texto é centralizado
 *          com base em scream_width; a altura começa em pos_y_inicial e
 *          avança conforme estilo.espacamento.
 *
 * @param fonte         Fonte a ser usada na renderização.
 * @param labels        Array de strings com os textos das opções.
 * @param total         Quantidade de opções no array.
 * @param opcao_atual   Índice da opção atualmente selecionada.
 * @param estilo        Cores, tamanhos e espaçamento do seletor.
 * @param pos_y_inicial Posição Y do primeiro item na tela.
 * @param scream_width  Largura da tela em pixels (usado para centralizar).
 */
void UiOpcaoDraw(Font fonte, const char **labels, int total, int opcao_atual,
                 UiOpcaoEstilo estilo, float pos_y_inicial, int scream_width);

#endif