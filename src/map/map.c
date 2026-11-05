/** @file map.c
 * @brief Implementação do módulo de mapa.
 */
#include "map/map.h"

// ─── API pública ──────────────────────────────────────────────────────────────

void MapInit(MapBody *self, MapType type, const char *texture_path) {
    self->type          = type;
    self->texture       = LoadTexture(texture_path);
    self->camera_offset = (Vector2){ 0.0f, 0.0f };
    self->crop.source   = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
    self->crop.dest     = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
}

void MapSetCrop(MapBody *self, Rectangle source, Rectangle dest) {
    self->crop.source = source;
    self->crop.dest   = dest;
}

void MapSetCamera(MapBody *self, Vector2 offset) {
    self->camera_offset = offset;
}

void MapDraw(MapBody *self) {
    switch (self->type) {

        case MAP_SINGLE:
            DrawTextureV(self->texture, self->camera_offset, WHITE);
            break;

        case MAP_CROP:
            DrawTexturePro(
                self->texture,
                self->crop.source,
                self->crop.dest,
                (Vector2){ 0.0f, 0.0f },
                0.0f,
                WHITE
            );
            break;

        case MAP_TILEMAP:
            // implementação futura
            break;

        default:
            break;
    }
}

void MapUnload(MapBody *self) {
    UnloadTexture(self->texture);
}