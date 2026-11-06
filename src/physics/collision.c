/**
 * @file collision.c
 * @brief Implementação do sistema de colisões 2D.
 */

#include "physics/collision.h"

bool CollisionCheck(Rectangle a, Rectangle b) {
    return CheckCollisionRecs(a, b);
}

void CollisionResolve(Rectangle *mover, Rectangle fixo) {
    if (!CheckCollisionRecs(*mover, fixo)) return;

    // calcula sobreposição em cada eixo
    float overlap_left   = (mover->x + mover->width)  - fixo.x;
    float overlap_right  = (fixo.x   + fixo.width)    - mover->x;
    float overlap_top    = (mover->y + mover->height)  - fixo.y;
    float overlap_bottom = (fixo.y   + fixo.height)    - mover->y;

    // empurra pelo menor overlap
    if (overlap_left < overlap_right && overlap_left < overlap_top && overlap_left < overlap_bottom) {
        mover->x -= overlap_left;
    } else if (overlap_right < overlap_left && overlap_right < overlap_top && overlap_right < overlap_bottom) {
        mover->x += overlap_right;
    } else if (overlap_top < overlap_bottom) {
        mover->y -= overlap_top;
    } else {
        mover->y += overlap_bottom;
    }
}

void CollisionDrawDebug(CollisionRect *colisores, int count) {
    for (int i = 0; i < count; i++) {
        DrawRectangleLinesEx(colisores[i].rect, 2.0f, RED);
        DrawText(colisores[i].tag,
            (int)colisores[i].rect.x + 4,
            (int)colisores[i].rect.y + 4,
            12, RED);
    }
}