/**
 * @file painel_tarefas.h
 * @brief Painel deslizante com a lista de tarefas da partida.
 * @details A aba "TAREFAS" fica sempre visível na borda esquerda da tela.
 *          Ao apertar TAB ou clicar na aba, o painel desliza para a direita e
 *          mostra a lista inteira, com um check nas tarefas já concluídas.
 *          O painel não pausa o jogo.
 */

#ifndef PAINEL_TAREFAS_H
#define PAINEL_TAREFAS_H

#include <raylib.h>
#include <stdbool.h>

/** @brief Quantidade de tarefas listadas no painel (linhas da arte). */
#define PAINEL_TOTAL_TAREFAS 5

/** @brief Duração da animação de abrir/fechar, em segundos. */
#define PAINEL_DURACAO_SLIDE 0.25f

// ── Calibração da arte ────────────────────────────────────────────────────────
// Todas as medidas abaixo estão em pixels da arte, na altura de referência.
// O painel é escalado por (altura da tela / PAINEL_ALTURA_REFERENCIA).

/** @brief Altura de tela em que a arte foi desenhada (escala 1:1). */
#define PAINEL_ALTURA_REFERENCIA 900.0f
/** @brief Distância do topo da tela até o topo do painel. */
#define PAINEL_POS_Y_REF         80.0f
/** @brief Largura da aba "TAREFAS" (parte que fica visível recolhida). */
#define PAINEL_TAB_LARGURA       55.0f
/** @brief Posição Y da aba, contada a partir do topo da arte. */
#define PAINEL_TAB_Y             5.0f
/** @brief Altura da aba. */
#define PAINEL_TAB_ALTURA        155.0f
/** @brief Centro X dos checks, contado a partir da esquerda da arte. */
#define PAINEL_CHECK_X           453.0f
/** @brief Centro Y do check da primeira linha, a partir do topo da arte. */
#define PAINEL_LINHA_Y0          48.0f
/** @brief Distância vertical entre as linhas de tarefa. */
#define PAINEL_LINHA_PASSO       55.0f

/** @struct PainelTarefasBody
 *
 * @brief Estado do painel deslizante de tarefas.
 *
 * @param painel    Arte do painel (corpo + aba), sem os checks.
 * @param check     Imagem do check de tarefa concluída.
 * @param progresso Posição da animação: 0 = recolhido (só a aba), 1 = aberto.
 * @param aberto    Destino da animação (true = abrindo/aberto).
 */
typedef struct PainelTarefasBody {
    Texture2D painel;
    Texture2D check;
    float     progresso;
    bool      aberto;
} PainelTarefasBody;

/** @brief Carrega as texturas e deixa o painel recolhido.
 * @param self Ponteiro para o PainelTarefasBody a ser inicializado.
 */
void PainelTarefasInit(PainelTarefasBody *self);

/**
 * @brief Processa TAB / clique na aba e avança a animação de deslize.
 *
 * @param self          Ponteiro para o PainelTarefasBody ativo.
 * @param scream_height Altura atual da tela em pixels (define a escala do painel).
 * @param som           Ponteiro para o Sound tocado ao abrir/fechar, ou NULL para silêncio.
 */
void PainelTarefasUpdate(PainelTarefasBody *self, int scream_height, Sound *som);

/**
 * @brief Desenha o painel na posição atual da animação, com os checks.
 *
 * @param self          Ponteiro para o PainelTarefasBody ativo.
 * @param concluidas    Vetor com PAINEL_TOTAL_TAREFAS posições, na ordem das
 *                      linhas da arte (true = tarefa concluída).
 * @param scream_height Altura atual da tela em pixels.
 */
void PainelTarefasDraw(PainelTarefasBody *self, const bool *concluidas, int scream_height);

/** @brief Descarrega as texturas do painel.
 * @param self Ponteiro para o PainelTarefasBody a ser descarregado.
 */
void PainelTarefasUnload(PainelTarefasBody *self);

#endif