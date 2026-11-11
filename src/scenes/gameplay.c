/**
 * @file gameplay.c
 * @brief Implementação da cena de gameplay.
 * @details Contém a lógica de inicialização, atualização e renderização
 *          da partida, incluindo o gerenciamento das telas de interface
 *          que se sobrepõem ao jogo (STATE_INTERFACE).
 */

#include "scenes/gameplay.h"
#include "core/game_state.h"
#include "graphics/ui.h"
#include "physics/collision.h"
#include "map/map.h"
#include "scenes/minigame_alucinacao.h"
#include <math.h> 

/** @brief Linhas exibidas na tela de instruções (edite livremente). */
static const char *INSTRUCOES_LINHAS[] = {
    "Use as setas para andar pelo mapa e navegar em interfaces",
    "Ache o computador do laboratorio e envie o relatorio correto",
    "ESC abre o menu de pausa (o tempo para)",
    "Clique M para ler o manual, ele ira te ajudar"
};
#define INSTRUCOES_TOTAL_LINHAS (int)(sizeof(INSTRUCOES_LINHAS) / sizeof(INSTRUCOES_LINHAS[0]))

/** @brief Desenha o cronômetro (MM:SS) no canto superior direito da tela. */
static void GameplayDrawHUD(GameplayBody *self, ResourcesBody *resources, int scream_width) {
    int total_seg = (int)ceilf(self->tempo_restante);
    if (total_seg < 0) total_seg = 0;

    const char *texto = TextFormat("%02d:%02d", total_seg / 60, total_seg % 60);
    const float tamanho = 60.0f;
    const float margem  = 30.0f;

    // Muda de cor conforme o tempo vai acabando
    Color cor = resources->branco;
    if (total_seg <= 10)      cor = RED;
    else if (total_seg <= 60) cor = resources->amarelo;

    Vector2 dim = MeasureTextEx(resources->fonte_principal, texto, tamanho, 1);
    DrawTextEx(resources->fonte_principal, texto,
               (Vector2){ scream_width - dim.x - margem, margem },
               tamanho, 1, cor);
}

/** @brief Desenha o overlay da tela de instruções. */
static void GameplayDrawInstrucoes(ResourcesBody *resources, int scream_width, int scream_height) {
    DrawRectangle(0, 0, scream_width, scream_height, (Color){ 0, 0, 0, 180 });

    const char *titulo = "INSTRUCOES";
    Vector2 titulo_dim = MeasureTextEx(resources->fonte_principal, titulo, 130.0f, 1);
    DrawTextEx(resources->fonte_principal, titulo,
               (Vector2){ (scream_width / 2.0f) - (titulo_dim.x / 2.0f), scream_height * 0.12f },
               130.0f, 1, resources->branco);

    float y = scream_height * 0.32f;
    for (int i = 0; i < INSTRUCOES_TOTAL_LINHAS; i++) {
        Vector2 dim = MeasureTextEx(resources->fonte_principal, INSTRUCOES_LINHAS[i], 50.0f, 1);
        DrawTextEx(resources->fonte_principal, INSTRUCOES_LINHAS[i],
                   (Vector2){ (scream_width / 2.0f) - (dim.x / 2.0f), y },
                   50.0f, 1, resources->branco);
        y += 70.0f;
    }

    const char *iniciar = "ENTER para comecar";
    Vector2 iniciar_dim = MeasureTextEx(resources->fonte_principal, iniciar, 72.0f, 1);
    DrawTextEx(resources->fonte_principal, iniciar,
               (Vector2){ (scream_width / 2.0f) - (iniciar_dim.x / 2.0f), scream_height * 0.85f },
               72.0f, 1, resources->amarelo);
}

/** @brief Caminhos dos PNGs do manual, na ordem das páginas (0 = capa). */
static const char *MANUAL_CAMINHOS[MANUAL_TOTAL_PAGINAS] = {
    "assets/texture/manual_jogador/manual_capa.png",
    "assets/texture/manual_jogador/manual_pag_1.png",
    "assets/texture/manual_jogador/manual_pag_2.png",
    "assets/texture/manual_jogador/manual_pag_3.png",
    "assets/texture/manual_jogador/manual_pag_4.png"
};

/** @brief Desconta o tempo da partida; ao zerar, encerra com derrota. */
static void GameplayTemporizadorUpdate(GameplayBody *self) {
    if (!self->timer_ativo) return;

    self->tempo_restante -= GetFrameTime();

    if (self->tempo_restante <= 0.0f) {
        self->tempo_restante   = 0.0f;
        self->timer_ativo = false;
        self->manual_aberto    = false;   // fecha o manual se o tempo acabar
        self->venceu           = false;
        self->terminou         = true;    // main troca para STATE_GAME_OVER
    }
}

/** @brief Processa o minigame de alucinação e reage ao resultado.
 *
 * @details Delega o input para AlucinacaoUpdate. Ao concluir, registra o
 *          minigame e fecha a interface. Ao cancelar, apenas fecha.
 *
 * @param self  Ponteiro para o GameplayBody ativo.
 * @param state Ponteiro para o estado global.
 */
static void GameplayUpdateMinigameAlucinacao(GameplayBody *self, GameState *state) {
    AlucinacaoUpdate(&self->alucinacao);

    if (self->alucinacao.concluido) {
        self->computador_usado = true;
        *state = STATE_PLAYING;
        GameplayMinigameConcluido(self, 0); // índice 0 = alucinação
        return;
    }

    if (self->alucinacao.cancelado) {
        self->alucinacao.cancelado = false;
        *state = STATE_PLAYING;
    }
}

/** @brief Desenha o minigame de alucinação sobre o jogo.
 *
 * @param self          Ponteiro para o GameplayBody ativo.
 * @param resources     Recursos visuais (fonte, cores).
 * @param scream_width  Largura da tela em pixels.
 * @param scream_height Altura da tela em pixels.
 */
static void GameplayDrawMinigameAlucinacao(GameplayBody *self, ResourcesBody *resources,
                                           int scream_width, int scream_height) {
    AlucinacaoDraw(&self->alucinacao, resources, scream_width, scream_height);
}

/** @brief Abre/fecha o manual (M) e troca de página (setas). */
static void GameplayManualUpdate(GameplayBody *self) {
    if (!self->manual_aberto) {
        if (IsKeyPressed(KEY_M)) {
            self->manual_aberto = true;
            self->manual_pagina_atual = 0;      // sempre abre na capa
        }
        return;
    }

    if (IsKeyPressed(KEY_M) || IsKeyPressed(KEY_ESCAPE)) {
        self->manual_aberto = false;
    } else if (IsKeyPressed(KEY_RIGHT) && self->manual_pagina_atual < MANUAL_TOTAL_PAGINAS - 1) {
        self->manual_pagina_atual++;
    } else if (IsKeyPressed(KEY_LEFT) && self->manual_pagina_atual > 0) {
        self->manual_pagina_atual--;
    }
}

/** @brief Desenha o manual: fundo escurecido, página atual e instruções de uso. */
static void GameplayDrawManual(GameplayBody *self, ResourcesBody *resources, int scream_width, int scream_height) {
    DrawRectangle(0, 0, scream_width, scream_height, (Color){ 0, 0, 0, 150 });

    // Página centralizada, mantendo a proporção. Começa abaixo do relógio (HUD)
    // e deixa a faixa de baixo livre para as instruções.
    Texture2D pag = self->manual_paginas[self->manual_pagina_atual];
    if (pag.id != 0) {
        float escala = fminf((scream_width * 0.90f) / pag.width,
                             (scream_height * 0.74f) / pag.height);
        float dest_w = pag.width  * escala;
        float dest_h = pag.height * escala;
        Rectangle origem  = { 0.0f, 0.0f, (float)pag.width, (float)pag.height };
        Rectangle destino = { (scream_width - dest_w) / 2.0f, scream_height * 0.10f, dest_w, dest_h };
        DrawTexturePro(pag, origem, destino, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
    } else {
        const char *caminho = MANUAL_CAMINHOS[self->manual_pagina_atual];
        const char *linhas[3] = {
            TextFormat("Imagem nao carregada: %s", caminho),
            TextFormat("Arquivo existe: %s", FileExists(caminho) ? "SIM" : "NAO"),
            TextFormat("Pasta de execucao: %s", GetWorkingDirectory())
        };
        for (int i = 0; i < 3; i++) {
            Vector2 dim = MeasureTextEx(resources->fonte_principal, linhas[i], 36.0f, 1);
            DrawTextEx(resources->fonte_principal, linhas[i],
                       (Vector2){ (scream_width / 2.0f) - (dim.x / 2.0f),
                                  scream_height * 0.40f + i * 50.0f },
                       36.0f, 1, RED);
        }
    }

    // Número da página
    const char *indice = TextFormat("Pagina %d/%d", self->manual_pagina_atual + 1, MANUAL_TOTAL_PAGINAS);
    Vector2 indice_dim = MeasureTextEx(resources->fonte_principal, indice, 50.0f, 1);
    DrawTextEx(resources->fonte_principal, indice,
               (Vector2){ (scream_width / 2.0f) - (indice_dim.x / 2.0f), scream_height * 0.86f },
               50.0f, 1, resources->branco);

    // Como usar / como sair
    const char *ajuda = "SETAS < > : mudar de pagina   |   M ou ESC : fechar o manual";
    Vector2 ajuda_dim = MeasureTextEx(resources->fonte_principal, ajuda, 50.0f, 1);
    DrawTextEx(resources->fonte_principal, ajuda,
               (Vector2){ (scream_width / 2.0f) - (ajuda_dim.x / 2.0f), scream_height * 0.92f },
               50.0f, 1, resources->amarelo);
}

/** @brief Desenha o overlay de confirmação de desistência.
 * @param self         Ponteiro para o GameplayBody.
 * @param resources    Recursos visuais (fonte, cores).
 * @param scream_width  Largura da tela em pixels.
 * @param scream_height Altura da tela em pixels.
 */
static void GameplayDrawVoltarMenu(GameplayBody *self, ResourcesBody *resources,
                                   int scream_width, int scream_height) {
    const char *labels[TOTAL_OPC] = { "DESISTIR", "VOLTAR AO JOGO" };

    const char *msg = "Certeza que quer desistir?";
    Vector2 msg_dim = MeasureTextEx(resources->fonte_principal, msg, 130.0f, 1);

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 0, 0, 0, 150 });
    DrawTextEx(resources->fonte_principal, msg,
        (Vector2){ (scream_width / 2.0f) - (msg_dim.x / 2.0f), scream_height * 0.30f },
        130.0f, 1, resources->branco);

    UiOpcaoEstilo estilo = {
        .size_normal  = 60.0f,
        .size_select  = 72.0f,
        .cor_normal   = resources->branco,
        .cor_select   = resources->amarelo,
        .espacamento  = 90.0f
    };
    UiOpcaoDraw(resources->fonte_principal, (const char **)labels, TOTAL_OPC,
                (int)self->opc_confirmacao, estilo, scream_height * 0.60f, scream_width);
}

// ─── Funções públicas ─────────────────────────────────────────────────────────

void GameplayInit(GameplayBody *self) {
    self->interface_atual = TELA_INSTRUCOES;
    self->tempo_conclusao       = 0;
    self->tempo_restante    = GAMEPLAY_TEMPO_TOTAL;
    self->timer_ativo  = false;
    self->instrucoes_vistas = false;

    self->manual_aberto = false;
    self->manual_pagina_atual = 0;
    TraceLog(LOG_INFO, "MANUAL: pasta de execucao = %s", GetWorkingDirectory());
    for (int i = 0; i < MANUAL_TOTAL_PAGINAS; i++) {
        self->manual_paginas[i] = LoadTexture(MANUAL_CAMINHOS[i]);
        TraceLog(LOG_INFO, "MANUAL: [%s] existe=%d textura_id=%d",
                 MANUAL_CAMINHOS[i], (int)FileExists(MANUAL_CAMINHOS[i]),
                 (int)self->manual_paginas[i].id);
    }

    AlucinacaoInit(&self->alucinacao);
    self->computador_usado = false;

    self->minigames_concluidos = 0;
    for (int i = 0; i < GAMEPLAY_TOTAL_MINIGAMES; i++) {
        self->minigames_completos[i] = false;
    }

    Vector2 centro_tela = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    Vector2 posicao_inicial = {2050 * MAP_ESCALA, 1050 * MAP_ESCALA};

    // ── Player (Estagiária) ───────────────────────────────────────────────────
    PlayerInit(&self->player, posicao_inicial);

    float sprite_w = PLAYER_BASE_WIDTH * PLAYER_BASE_SCALE;  
    float sprite_h =  PLAYER_BASE_HEIGHT * PLAYER_BASE_SCALE;

    // ── Mapa do jogo ──────────────────────────────────────────────────────────
    MapInit(&self->map, MAP_SINGLE, "assets/texture/mapa/mapa.png"); 
    self->camera.target   = (Vector2){ 
        self->player.position.x + sprite_w / 2.0f,
        self->player.position.y + sprite_h / 2.0f };  // o que a câmera mira
    self->camera.offset   = (Vector2)centro_tela; 
    self->camera.rotation = 0.0f;
    self->camera.zoom     = 1.0f;

    self->interface_atual = TELA_VOLTAR_MENU;
    self->tempo_conclusao       = 0;
    self->nickname[0] = '\0';
    self->terminou    = false;
    self->venceu      = false;
    self->voltar_menu = false;
    self->opc_confirmacao = VOLTAR_MENU;
}

void GameplayUpdate(GameplayBody *self, GameState *state) {

     if (*state == STATE_INTERFACE) {

        switch (self->interface_atual) {
            case TELA_VOLTAR_MENU:
                GameplayTemporizadorUpdate(self);

                int confirmado = UiOpcaoUpdate((int*)&self->opc_confirmacao, TOTAL_OPC);
                if (confirmado == VOLTAR_MENU) {
                    self->voltar_menu = true; // SIM
                } else if (confirmado == VOLTAR_JOGO) {
                    self->opc_confirmacao = VOLTAR_MENU;
                    *state = STATE_PLAYING; // NAO
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    self->opc_confirmacao = VOLTAR_MENU;
                    *state = STATE_PLAYING;
                }
                break;

            case TELA_MINIGAME_ALUCINACAO:
                GameplayTemporizadorUpdate(self);
                GameplayUpdateMinigameAlucinacao(self, state);
                break;
                    
            case TELA_INSTRUCOES: 
                if (IsKeyPressed(KEY_ENTER)) {
                    self->instrucoes_vistas = true;
                    self->timer_ativo  = true;  // o tempo começa a contar aqui
                    *state = STATE_PLAYING;
                }
                break;
            
            default:
                break;
        }

        return;  // trava o resto do update
    }

    // ── Manual (M): o tempo continua, mas player e câmera ficam congelados ──
    if (self->manual_aberto || IsKeyPressed(KEY_M)) {
        GameplayManualUpdate(self);
        GameplayTemporizadorUpdate(self);
        return;
    }

    // Aqui o player escolhe se volta pro menu ou não
    if (IsKeyPressed(KEY_ESCAPE)) {
        self->interface_atual = TELA_VOLTAR_MENU;
        *state = STATE_INTERFACE; 
        return;
    }

    // ── Temporizador ──
    GameplayTemporizadorUpdate(self);
    if (self->terminou) return;

    // ── Interação com o computador (minigame de alucinação) ──

    Vector2 player_centro = {
        self->player.position.x + (PLAYER_BASE_WIDTH  * PLAYER_BASE_SCALE) / 2.0f,
        self->player.position.y + (PLAYER_BASE_HEIGHT * PLAYER_BASE_SCALE) / 2.0f
    };

    if (!self->computador_usado && AlucinacaoPoderInteragir(&self->alucinacao, player_centro) && IsKeyPressed(KEY_E)) {
        AlucinacaoInit(&self->alucinacao);
        self->interface_atual = TELA_MINIGAME_ALUCINACAO;
        *state = STATE_INTERFACE;
        return;
    }

    PlayerUpdateAction(&self->player, self->map.colisores, self->map.colisor_count);


    // Câmera andando junto com o player
    float sprite_w = PLAYER_BASE_WIDTH * PLAYER_BASE_SCALE;  
    float sprite_h =  PLAYER_BASE_HEIGHT * PLAYER_BASE_SCALE;

    self->camera.target = (Vector2){
        self->player.position.x + sprite_w / 2.0f,
        self->player.position.y + sprite_h / 2.0f
    };
}

void GameplayDraw(GameplayBody *self, GameState *state,  ResourcesBody *resources, int scream_width, int scream_height) {
    BeginMode2D(self->camera);
        MapDraw(&self->map);
        AlucinacaoDrawMundo(&self->alucinacao);
        PlayerDraw(&self->player);
        //CollisionDrawDebug(self->map.colisores, self->map.colisor_count); // ← debug
    EndMode2D();
    //debug
    //Vector2 world = GetScreenToWorld2D(GetMousePosition(), self->camera);
    //DrawText(TextFormat("x: %.0f  y: %.0f", world.x / MAP_ESCALA, world.y / MAP_ESCALA), 10, 10, 24, GREEN);

    Vector2 player_centro = {
        self->player.position.x + (PLAYER_BASE_WIDTH  * PLAYER_BASE_SCALE) / 2.0f,
        self->player.position.y + (PLAYER_BASE_HEIGHT * PLAYER_BASE_SCALE) / 2.0f
    };

    // Indicador de interação do computador
    if (!self->computador_usado && AlucinacaoPoderInteragir(&self->alucinacao, player_centro)) {
        AlucinacaoDrawIndicador(&self->alucinacao, resources, self->camera);
    }

    if (*state == STATE_PLAYING && self->manual_aberto) {
        GameplayDrawManual(self, resources, scream_width, scream_height);
    }

    GameplayDrawHUD(self, resources, scream_width); 

    if (*state == STATE_INTERFACE) {
        switch (self->interface_atual) {
            case TELA_VOLTAR_MENU:
                GameplayDrawVoltarMenu(self, resources, scream_width, scream_height);
                break;

            case TELA_MINIGAME_ALUCINACAO:
                GameplayDrawMinigameAlucinacao(self, resources, scream_width, scream_height);
                break;

            case TELA_INSTRUCOES:                   
                GameplayDrawInstrucoes(resources, scream_width, scream_height);
                break;
            
            default:
                break;
        }
    }
}

void GameplayMinigameConcluido(GameplayBody *self, int indice) {
    if (indice < 0 || indice >= GAMEPLAY_TOTAL_MINIGAMES) return;
    if (self->minigames_completos[indice]) return; // já contabilizado

    self->minigames_completos[indice] = true;
    self->minigames_concluidos++;

    if (self->minigames_concluidos >= GAMEPLAY_TOTAL_MINIGAMES) {
        GameplayVencer(self);
    }
}

void GameplayVencer(GameplayBody *self) {
    self->timer_ativo = false;
    self->manual_aberto    = false;
    self->tempo_conclusao  = (int)(GAMEPLAY_TEMPO_TOTAL - self->tempo_restante);  // tempo decorrido em segundos
    self->venceu   = true;
    self->terminou = true;
}

void GameplayUnload(GameplayBody *self) {
    for (int i = 0; i < MANUAL_TOTAL_PAGINAS; i++) {
        UnloadTexture(self->manual_paginas[i]);
    }
    PlayerUnload(&self->player);
    MapUnload(&self->map);
    AlucinacaoUnload(&self->alucinacao);
}