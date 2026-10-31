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
    if (IsKeyPressed(KEY_ESCAPE)) {
        self->voltar_menu = true;
        return;
    }

    if (*state == STATE_INTERFACE) return;

    PlayerUpdateAction(&self->player);

    float sprite_w = PLAYER_BASE_WIDTH * PLAYER_BASE_SCALE;  
    float sprite_h =  PLAYER_BASE_HEIGHT * PLAYER_BASE_SCALE;

    self->camera.target = (Vector2){
        self->player.position.x + sprite_w / 2.0f,
        self->player.position.y + sprite_h / 2.0f
    };
}

void GameplayDraw(GameplayBody *self) {
    BeginMode2D(self->camera);
        MapDraw(&self->map);
        PlayerDraw(&self->player);
    EndMode2D();
}

void GameplayUnload(GameplayBody *self) {
    PlayerUnload(&self->player);
    MapUnload(&self->map);
}