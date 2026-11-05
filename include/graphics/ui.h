/**
 * @file ui.h
 * @brief Componentes de interface de usuário reutilizáveis.
 * @details Funções genéricas para renderizar e processar input de elementos
 *          de UI (seletores de opções, etc.) que se repetem em múltiplas cenas.
 */

#ifndef UI_H
#define UI_H

#include <raylib.h>

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
 * @brief Processa o input de navegação de um seletor de opções.
 *
 * @details Escuta KEY_UP e KEY_DOWN para mover o índice (com wrap circular)
 *          e KEY_ENTER para confirmar. Teclas específicas de cada cena
 *          (como KEY_ESCAPE) devem ser tratadas pelo código chamador.
 *
 * @param opcao_atual  Ponteiro para o índice atual — atualizado internamente.
 * @param total        Quantidade total de opções.
 * @return             Índice da opção confirmada com ENTER, ou -1 se nada foi confirmado.
 */
int UiOpcaoUpdate(int *opcao_atual, int total);

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