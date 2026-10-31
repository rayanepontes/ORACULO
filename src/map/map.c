/** @file map.c
 * @brief Implementação do módulo de mapa.
 */
#include "map/map.h"

// ─── API pública ──────────────────────────────────────────────────────────────

void MapInit(MapBody *self, MapType type, const char *texture_path) {
    self->type          = type;
    self->texture       = LoadTexture(texture_path);
    self->crop.source   = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
    self->crop.dest     = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
}

void MapSetCrop(MapBody *self, Rectangle source, Rectangle dest) {
    self->crop.source = source;
    self->crop.dest   = dest;
}

void MapDraw(MapBody *self) {
    switch (self->type) {

        case MAP_SINGLE:
            float escala = 2.0f;  // ajuste até ficar bom visualmente
            
            DrawTextureEx(self->texture, (Vector2){0, 0}, 0.0f, escala, WHITE);
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