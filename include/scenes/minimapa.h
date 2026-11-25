/**
 * @file minimapa.h
 * @brief Minimapa de tarefas: mostra quais salas ainda têm tarefa pendente.
 * @details O estado de cada sala é DERIVADO das flags já existentes no
 *          GameplayBody (nada é duplicado). Texturas carregadas uma única vez
 *          no Init e descarregadas no Unload.
 */

#ifndef MINIMAPA_H
#define MINIMAPA_H

#include <raylib.h>
#include <stdbool.h>
#include "core/resources.h"

// ─── Arquivos ─────────────────────────────────────────────────────────────────
#define MINIMAPA_BASE_PATH        "assets/texture/mapa/minimapa_base.png"
#define MINIMAPA_TRIANGULO_PATH   "assets/texture/mapa/task_icon.png"
#define MINIMAPA_ICONE_PATH       "assets/texture/mapa/minimapa_icon.png"
#define MINIMAPA_ICONE_HOVER_PATH "assets/texture/mapa/minimapa_icone_hover.png"
#define MINIMAPA_CAFE_ICON_PATH "assets/texture/mapa/cafe_icon.png"

// ─── Tecla de atalho ──────────────────────────────────────────────────────────
/** @brief Tecla que abre/fecha o minimapa (M = manual, E = interagir, TAB = tarefas). */
#define MINIMAPA_TECLA KEY_N

// ─── Coordenadas de referência do DESIGN do mapa ──────────────────────────────
/** @brief Tamanho do design do mapa em pixels. TODO: confirmar o tamanho real do PNG. */
#define MINIMAPA_REF_W 3866.0f
#define MINIMAPA_REF_H 2169.0f

/** @brief Tamanho do triângulo no design, em pixels. TODO: ajustar. */
#define MINIMAPA_TRI_W 248.0f
#define MINIMAPA_TRI_H 248.0f

/** @brief Tamanho do ícone do café (Copa) no design, em pixels. TODO: ajustar. */
#define MINIMAPA_CAFE_W 169.51f
#define MINIMAPA_CAFE_H 169.51f

/** @brief Tamanho da fonte dos nomes no design. TODO: ajustar até caber nas salas. */
#define MINIMAPA_FONTE_REF 72.0f

/** @brief Fração máxima da tela ocupada pelo mapa (deixa folga para o ícone à direita). */
#define MINIMAPA_MAPA_MAX_W 0.84f
#define MINIMAPA_MAPA_MAX_H 0.78f

/** @brief Velocidade do fade do triângulo (1.0 = um segundo; use 100.0f para desligar). */
#define MINIMAPA_FADE_VELOCIDADE 2.5f

// ─── Ícone na gameplay (referência 1920x1080) ─────────────────────────────────
#define MINIMAPA_TELA_REF_W        1920.0f
#define MINIMAPA_TELA_REF_H        1080.0f
/** @brief Lado do ícone. TODO: ajustar ao tamanho desejado. */
#define MINIMAPA_ICONE_LADO_REF    96.0f
/** @brief Distância da borda direita (igual à margem do cronômetro). */
#define MINIMAPA_ICONE_MARGEM_REF  30.0f
/** @brief Topo do ícone: abaixo do cronômetro (y=30) e do HUD do café (y=100, tam 40). TODO: ajustar. */
#define MINIMAPA_ICONE_Y_REF       175.0f
/** @brief Topo mínimo em pixels reais (o HUD usa pixels fixos, então isso evita sobreposição). */
#define MINIMAPA_ICONE_Y_MIN_PX    150.0f

/** @enum SalaMinimapa
 * @brief Salas exibidas no minimapa (cada uma tem exatamente uma tarefa).
 */
typedef enum SalaMinimapa {
    MINIMAPA_SALA_COFRE,       ///< Reconector neural.
    MINIMAPA_SALA_ESCRITORIO,  ///< Detector de alucinação.
    MINIMAPA_SALA_COPA,        ///< Cafeteira.
    MINIMAPA_SALA_RECEPCAO,    ///< Botão do alarme.
    MINIMAPA_SALA_FORMACAO,    ///< Triador de treinamento.
    MINIMAPA_SALA_SERVIDORES,  ///< Alavanca de emergência.
    MINIMAPA_TOTAL_SALAS       ///< Quantidade total de salas.
} SalaMinimapa;

/** @struct MinimapaBody
 * @brief Texturas e estado visual do minimapa.
 * @param base          Formas das salas (sem textos e sem triângulos).
 * @param triangulo     Triângulo de alerta.
 * @param icone         Ícone circular do canto da tela.
 * @param icone_hover   Ícone com o mouse em cima (opcional).
 * @param alpha_alerta  Opacidade atual do triângulo de cada sala (só visual).
 */
typedef struct MinimapaBody {
    Texture2D base;
    Texture2D triangulo;
    Texture2D icone_cafe;   ///< Ícone usado só na Copa no lugar do triângulo.
    Texture2D icone;
    Texture2D icone_hover;
    float     alpha_alerta[MINIMAPA_TOTAL_SALAS];
} MinimapaBody;

/** @brief Carrega as texturas (chamar UMA vez por partida). */
void MinimapaInit(MinimapaBody *self);

/** @brief Diz quais salas ainda têm tarefa pendente a partir das flags do gameplay.
 * @param saida Vetor com MINIMAPA_TOTAL_SALAS posições; true = pendente.
 */
void MinimapaCalcularPendentes(bool computador_usado, bool triador_usado,
                               bool alavanca_usada, bool reconector_usado,
                               bool botao_usado, bool cafeteira_usada,
                               bool saida[MINIMAPA_TOTAL_SALAS]);

/** @brief Anima o fade dos triângulos. Chamar só com o mapa aberto. */
void MinimapaUpdate(MinimapaBody *self, const bool pendentes[MINIMAPA_TOTAL_SALAS]);

/** @brief True no frame em que o jogador clica com o botão esquerdo no ícone. */
bool MinimapaIconeClicado(int screen_width, int screen_height);

/** @brief Desenha o ícone do canto da tela. @param aberto true destaca o ícone (mapa aberto). */
void MinimapaDrawIcone(MinimapaBody *self, int screen_width, int screen_height, bool aberto);

/** @brief Desenha o overlay do mapa (fundo escurecido, salas, nomes e triângulos). */
void MinimapaDraw(MinimapaBody *self, ResourcesBody *resources, int screen_width, int screen_height);

/** @brief Descarrega as texturas. */
void MinimapaUnload(MinimapaBody *self);

#endif