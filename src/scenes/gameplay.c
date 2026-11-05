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

void GameplayInit(GameplayBody *self) {
    Vector2 centro_tela = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
    //Vector2 centro_mapa = {self->map.texture.width / 2, self->map.texture.height / 2};

    // ── Player (Estagiária) ───────────────────────────────────────────────────
    PlayerInit(&self->player, centro_tela);

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

    PlayerUpdateAction(&self->player);


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
    EndMode2D();

    if (*state == STATE_INTERFACE) {
        switch (self->interface_atual) {
            case TELA_VOLTAR_MENU: {
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
            }
            
            default:
                break;
        }
    }
}

void GameplayUnload(GameplayBody *self) {
    PlayerUnload(&self->player);
    MapUnload(&self->map);
}