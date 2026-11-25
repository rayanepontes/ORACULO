/**
 * @file resources.h
 * @brief Recursos globais compartilhados por todas as cenas do jogo.
 * @details Centraliza informações que precisam estar disponíveis durante toda
 *          a execução. Deve ser inicializado uma única vez
 *          no main e descarregado ao encerrar o jogo.
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#include <raylib.h>
#include "database/database.h"

/** @brief Caminho para o arquivo de fonte principal. */
#define RESOURCES_FONTE_PATH "assets/fonts/VT323/VT323-Regular.ttf"

/** @brief Tamanho em pontos da fonte principal. */
#define RESOURCES_FONTE_SIZE 64

/** @brief Caminho para a música do menu. */
#define RESOURCES_MUSICA_MENU_PATH     "assets/audio/music/oraculo_menu_music.wav"

/** @brief Caminho para a música do gameplay. */
#define RESOURCES_MUSICA_GAMEPLAY_PATH "assets/audio/music/oraculo_gameplay_music.wav"

/** @brief Caminho para o som de alarme. */
#define RESOURCES_SOM_ALARME_PATH "assets/audio/sfx/alarm_sound.wav"

/** @brief Caminho para o efeito sonoro de troca de opção nos menus. */
#define RESOURCES_SOM_SELECAO_PATH     "assets/audio/sfx/select_sound.wav"

/** @brief Caminho para o efeito sonoro de confirmação de seleção nos menus. */
#define RESOURCES_SOM_CONFIRMACAO_PATH "assets/audio/sfx/selected_confirm_sound.wav"

/** @brief Caminho para o efeito sonoro de game over. */
#define RESOURCES_SOM_GAMEOVER_PATH "assets/audio/sfx/gameover_sound.wav"

/** @brief Caminho para o efeito sonoro de vitória. */
#define RESOURCES_SOM_WIN "assets/audio/sfx/win_sound.wav"

/** @brief Caminho para o efeito sonoro de entrada em minigames. */
#define RESOURCES_SOM_MINIGAME_ENTRY "assets/audio/sfx/minigame_entry.wav"

/** @brief Volume das músicas de fundo do menu (0.0 a 1.0). */
#define RESOURCES_MUSICA_MENU_VOLUME 0.6f

/** @brief Volume do alarme (0.0 a 1.0). */
#define RESOURCES_ALARME_VOLUME 0.4f

/** @brief Volume das músicas de fundo da gameplay (0.0 a 1.0). */
#define RESOURCES_MUSICA_GAMEPLAY_VOLUME 0.3f

/** @brief Volume dos efeitos sonoros (0.0 a 1.0). */
#define RESOURCES_SFX_VOLUME 0.8f

/**
 * @struct ResourcesBody
 * @brief Recursos globais compartilhados por todas as cenas.
 * @param fonte_principal    Fonte VT323, usada em todas as cenas.
 * @param ranking            Entradas do ranking carregadas do arquivo.
 * @param ranking_count      Número de entradas válidas em ranking.
 * @param amarelo            Cor amarela da paleta do jogo.
 * @param branco             Branco puro.
 * @param lilas_escuro       Lilás escuro da paleta.
 * @param lilas_claro        Lilás claro da paleta.
 * @param ciano_claro        Ciano clarinho.
 * @param letra_caixa        Cor das letras na caixa de nome.
 * @param musica_menu        Musica tema do menu.
 * @param musica_gameplay    Musica tema da gameplay.
 * @param alarme             Alarme que toca até que a pessoa segure o botão.
 * @param som_selecao        Efeito sonoro tocado ao navegar entre opções de menu.
 * @param som_confirmacao    Efeito sonoro tocado ao confirmar uma opção (ENTER).
 * @param som_gameover       Efeito sonoro tocado ao entrar na tela de game over.
 * @param som_win            Efeito sonoro tocado ao entrar na tela de vitória.
 * @param som_entry_minigame Efeito sonoro tocado ao entrar em um minigame qualquer.
 */
typedef struct ResourcesBody {
    Font     fonte_principal;
    GameData ranking[DATABASE_MAX_ENTRIES];
    int      ranking_count;
    Color    amarelo;
    Color    branco;
    Color    azul_escuro;
    Color    lilas_escuro;
    Color    lilas_claro;
    Color    ciano_claro; 
    Color    letra_caixa;
    Music    musica_menu;
    Music    musica_gameplay;
    Music    alarme;
    Sound    som_selecao;
    Sound    som_confirmacao;
    Sound    som_gameover;
    Sound    som_win;
    Sound    som_entry_minigame;
} ResourcesBody;

/**
 * @brief Inicializa e carrega todos os recursos globais.
 * @param self Ponteiro para o ResourcesBody a ser inicializado.
 */
void ResourcesInit(ResourcesBody *self);

/**
 * @brief Descarrega todos os recursos globais da memória.
 * @param self Ponteiro para o ResourcesBody a ser descarregado.
 */
void ResourcesUnload(ResourcesBody *self);

#endif