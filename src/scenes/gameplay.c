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

void GameplayInit(GameplayBody *self) {
    self->interface_atual = TELA_INSTRUCOES;
    self->score       = 0;
    self->tempo_restante    = GAMEPLAY_TEMPO_TOTAL;
    self->cronometro_ativo  = false;
    self->instrucoes_vistas = false;

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
    self->score       = 0;
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
                    
            case TELA_INSTRUCOES: 
                if (IsKeyPressed(KEY_ENTER)) {
                    self->instrucoes_vistas = true;
                    self->cronometro_ativo  = true;  // o tempo começa a contar aqui
                    *state = STATE_PLAYING;
                }
                break;
            
            default:
                break;
        }

        return;  // trava o resto do update
    }

    // Aqui o player escolhe se volta pro menu ou não
    if (IsKeyPressed(KEY_ESCAPE)) {
        self->interface_atual = TELA_VOLTAR_MENU;
        *state = STATE_INTERFACE; 
        return;
    }

    // ── Cronômetro regressivo ────────────────────────────────────────────────
    // Só chega aqui em STATE_PLAYING; em STATE_INTERFACE o return acima o pausa.
    if (self->cronometro_ativo) {
        self->tempo_restante -= GetFrameTime();

        if (self->tempo_restante <= 0.0f) {
            self->tempo_restante   = 0.0f;
            self->cronometro_ativo = false;
            self->venceu           = false;
            self->terminou         = true;   // main troca para STATE_GAME_OVER
            return;
        }
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
        PlayerDraw(&self->player);
        //CollisionDrawDebug(self->map.colisores, self->map.colisor_count); // ← debug
    EndMode2D();

    GameplayDrawHUD(self, resources, scream_width); 

    if (*state == STATE_INTERFACE) {
        switch (self->interface_atual) {
            case TELA_VOLTAR_MENU:
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
                break;

            case TELA_INSTRUCOES:  
                                
                GameplayDrawInstrucoes(resources, scream_width, scream_height);
                break;
            
            default:
                break;
        }
    }
}

void GameplayVencer(GameplayBody *self) {
    self->cronometro_ativo = false;
    self->score    = (int)self->tempo_restante;  // tempo que sobrou vira pontuação
    self->venceu   = true;
    self->terminou = true;
}

void GameplayUnload(GameplayBody *self) {
    PlayerUnload(&self->player);
    MapUnload(&self->map);
}