/**
 * @file gameplay.c
 * @brief Implementação da cena de gameplay.
 */

#include "scenes/gameplay.h"
#include "core/game_state.h"

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

    self->score       = 0;
    self->nickname[0] = '\0';
    self->terminou    = false;
    self->venceu      = false;
    self->voltar_menu = false;
}

void GameplayUpdate(GameplayBody *self, GameState *state) {

     if (*state == STATE_INTERFACE) {

        switch (self->interface_atual) {
            case TELA_VOLTAR_MENU:
                // tela de confirmação ativa
                if (IsKeyPressed(KEY_ENTER)) {
                    self->voltar_menu = true;
                    return;
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    *state = STATE_PLAYING;  // cancela, volta ao jogo
                }
                break;
            
            default:
                break;
        }

        return;  // trava o resto do update
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        *state = STATE_INTERFACE;  // abre confirmação
        return;
    }

    PlayerUpdateAction(&self->player);

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
            case TELA_VOLTAR_MENU:
                const char *msg  = "Certeza que quer desistir?";
                const char *instrucao = "ENTER - sim   ESC - voltar";

                Vector2 msg_dimension  = MeasureTextEx(resources->fonte_principal, msg, 130.0f, 1);
                Vector2 instrucao_dimension = MeasureTextEx(resources->fonte_principal, instrucao, 50.0f, 1);

                // sobreposição de confirmação
                DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){ 0, 0, 0, 150 }); // fundo cinza
                DrawTextEx(resources->fonte_principal, msg,
                    (Vector2){ (scream_width / 2.0f) - (msg_dimension.x / 2.0f), scream_height * 0.45f },
                    130.0f, 1, resources->branco);
                DrawTextEx(resources->fonte_principal, instrucao,
                    (Vector2){ (scream_width / 2.0f) - (instrucao_dimension.x / 2.0f), scream_height * 0.65f },
                    50.0f, 1, resources->lilas_claro);               
                break;
            
            default:
                break;
        }
    }
}

void GameplayUnload(GameplayBody *self) {
    PlayerUnload(&self->player);
    MapUnload(&self->map);
}