/**
 * @file alucinacao.h
 * @brief Minigame do Detector de Alucinação.
 * @details O jogador navega entre relatórios e escolha o correto com base
 *          no manual. Fica aberto até acertar ou cancelar (ESC).
 */

#ifndef ALUCINACAO_H
#define ALUCINACAO_H

#include <raylib.h>
#include <stdbool.h>
#include "core/interagivel.h"
#include "core/resources.h"

/** @brief Total de relatórios exibidos no minigame. */
#define ALUCINACAO_TOTAL_RELATORIOS 3

/** @brief Índice (0-based) do relatório correto. */
#define ALUCINACAO_RELATORIO_CORRETO 0

/** @enum TelaAlucinacao
 * @brief Subtelas internas do minigame de alucinação.
 *
 * @param TELA_ALUC_RELATORIOS    Navegação entre os relatórios (◄ ►).
 * @param TELA_ALUC_CONFIRMACAO   Confirmação antes de enviar o relatório escolhido.
 * @param TELA_ALUC_ERRO          Mensagem de erro ao escolher o relatório errado.
 */
typedef enum TelaAlucinacao {
    TELA_ALUC_RELATORIOS,  // Navegação entre os relatórios
    TELA_ALUC_CONFIRMACAO, // Confirmação de escolha
    TELA_ALUC_ERRO         // Feedback de resposta errada
} TelaAlucinacao;

/** @enum OpcConfirmacaoAluc
 * @brief Opções da tela de confirmação do minigame.
 *
 * @param ALUC_CONFIRMAR  Jogador confirma o relatório escolhido.
 * @param ALUC_CANCELAR   Jogador volta para a navegação.
 * @param ALUC_TOTAL_OPC  Total de opções.
 */
typedef enum OpcConfirmacaoAluc {
    ALUC_CONFIRMAR,  // Confirmar escolha
    ALUC_CANCELAR,   // Voltar para os relatórios
    ALUC_TOTAL_OPC   // Total de opções
} OpcConfirmacaoAluc;

/** @struct AlucinacaoBody
 * @brief Estado completo do minigame de alucinação.
 *
 * @param tela_atual          Subtela ativa no momento.
 * @param relatorio_atual     Índice do relatório visível (0 a ALUCINACAO_TOTAL_RELATORIOS-1).
 * @param relatorio_correto   Índice da resposta certa.
 * @param opc_confirmacao     Opção selecionada na tela de confirmação.
 * @param concluido           True quando o jogador acerta — gameplay lê para registrar vitória.
 * @param cancelado           True quando o jogador sai com ESC — gameplay lê para fechar.
 * @param relatorios          Texturas dos relatórios carregadas do disco.
 * @param background          Textura do fundo do minigame.
 * @param computador_sprite  Sprite do computador exibido no mapa.
 * @param computador  Objeto interativo do computador (posição, tamanho e área de interação).
 */
typedef struct AlucinacaoBody {
    TelaAlucinacao    tela_atual;
    int               relatorio_atual;
    int               relatorio_correto;
    OpcConfirmacaoAluc opc_confirmacao;
    bool              concluido;
    bool              cancelado;
    Texture2D         relatorios[ALUCINACAO_TOTAL_RELATORIOS];
    Texture2D         background; 
    Interagivel       computador;   
} AlucinacaoBody;

/** @brief Inicializa o minigame carregando as texturas dos relatórios.
 * @param self Ponteiro para o AlucinacaoBody a ser inicializado.
 */
void AlucinacaoInit(AlucinacaoBody *self);

/** @brief Processa o input do jogador dentro do minigame.
 * @param self Ponteiro para o AlucinacaoBody ativo.
 */
void AlucinacaoUpdate(AlucinacaoBody *self);

/** @brief Desenha o indicador de interação acima do computador (chamar fora de BeginMode2D).
 * @param self          Ponteiro para o AlucinacaoBody ativo.
 * @param resources     Recursos visuais (fonte, cores).
 * @param camera        Câmera 2D atual.
 * @param scream_width  Largura da tela em pixels.
 */
void AlucinacaoDrawIndicador(AlucinacaoBody *self, ResourcesBody *resources, Camera2D camera);

/** @brief Desenha o minigame na tela.
 * @param self          Ponteiro para o AlucinacaoBody ativo.
 * @param resources     Recursos visuais (fonte, cores).
 * @param scream_width  Largura da tela em pixels.
 * @param scream_height Altura da tela em pixels.
 */
void AlucinacaoDraw(AlucinacaoBody *self, ResourcesBody *resources,
                    int scream_width, int scream_height);

/** @brief Descarrega as texturas do minigame da memória.
 * @param self Ponteiro para o AlucinacaoBody a ser descarregado.
 */
void AlucinacaoUnload(AlucinacaoBody *self);

#endif