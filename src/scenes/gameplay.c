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

    // ── Mapa do jogo ──────────────────────────────────────────────────────────
    MapInit(&self->map, MAP_SINGLE, "assets/texture/mapa/mapa-temporario.png"); 
    self->camera.target   = self->player.position;  // o que a câmera mira
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

    self->camera.target = self->player.position;
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