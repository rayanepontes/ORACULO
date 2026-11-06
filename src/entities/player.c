#include <raylib.h>
#include "entities/player.h"

//----------- Funções internas (não expostas no .h) - Essas são de uso exclusivo desse arquivo .c

//Inicializa as animações carregando as texturas e montando os corpos dos tipos de animações do player.
static void InitAnimations(PlayerBody *self){
    self->textures[idle] = LoadTexture("assets/texture/player_sprites/Personagem_Principal_Idle_4.png");
    self->anims[idle] = (AnimationBody){
    .first_idx        = 0,
    .last_idx         = 3,
    .frame_atual_idx  = 0,
    .step             = 1,
    .speed            = 0.1f,
    .duration_left    = 0.1f,
    .frames_por_linha = 4,
    .spr_width        = 175.0f,  // largura total ÷ 4
    .spr_height       = 257.0f,  // altura total da imagem
    .type             = REPEATING
    };

    self->textures[walk] = LoadTexture("assets/texture/player_sprites/Personagem_Principal_Walk_5.png");
    self->anims[walk] = (AnimationBody){
    .first_idx        = 0,
    .last_idx         = 4,       // 5 frames: índices 0 a 4
    .frame_atual_idx  = 0,
    .step             = 1,
    .speed            = 0.1f,
    .duration_left    = 0.1f,
    .frames_por_linha = 5,
    .spr_width        = 177.6f,  // largura total ÷ 5
    .spr_height       = 274.0f,  // altura total da imagem
    .type             = REPEATING
    };

    self->textures[run] = LoadTexture("assets/texture/sprites_teste/tiny_heroes_spr/3 Dude_Monster/Dude_Monster_Run_6.png");
    self->anims[run] = (AnimationBody){
            .first_idx = 0,
            .last_idx = 5,
            .frame_atual_idx = 0,
            .step = 1,
            .speed = 0.1,
            .duration_left = 0.1,
            .frames_por_linha = 5,
            .spr_width = 32.0,
            .spr_height = 32.0,           
            .type = REPEATING
        };
}

//Reseta o quadro de todas as animações do player para o estado inicial delas.
void ResetAnimations(PlayerBody *self){
    for (int i = 0; i < total_actions; i++){
        self->anims[i].frame_atual_idx = self->anims[i].first_idx;
        self->anims[i].duration_left = self->anims[i].speed;
    }
}

// -------- Funções visiveis para qualquer lugar que importalas.

void PlayerInit(PlayerBody *self, Vector2 posicao_inicial){
    InitAnimations(self);

    self->position = posicao_inicial;
    self->direction = RIGHT;
    self->action_atual = idle;
}

void PLayerReset(PlayerBody *self, Vector2 posicao_inicial){
    //Texturas já estão carregadas, só reseta estado.
    ResetAnimations(self);

    self->position     = posicao_inicial;
    self->direction    = RIGHT;
    self->action_atual = idle;
}

void PlayerUpdateAction(PlayerBody *self, CollisionRect *colisores, int colisor_count) {
    float player_speed = 4.0f;

    bool movendo  = false;
    bool correndo = false;

    float vel_x = 0.0f;
    float vel_y = 0.0f;

    // ── Calcula velocidade desejada ───────────────────────────────────────────
    if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
        self->direction = LEFT;
        vel_x = IsKeyDown(KEY_LEFT_SHIFT) ? -(player_speed * 2) : -player_speed;
        correndo = IsKeyDown(KEY_LEFT_SHIFT);
        movendo  = !correndo;
    } else if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {
        self->direction = RIGHT;
        vel_x = IsKeyDown(KEY_LEFT_SHIFT) ? (player_speed * 2) : player_speed;
        correndo = IsKeyDown(KEY_LEFT_SHIFT);
        movendo  = !correndo;
    }

    if (IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {
        vel_y = IsKeyDown(KEY_LEFT_SHIFT) ? -(player_speed * 2) : -player_speed;
        correndo = IsKeyDown(KEY_LEFT_SHIFT);
        movendo  = movendo || !correndo;
    } else if (IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) {
        vel_y = IsKeyDown(KEY_LEFT_SHIFT) ? (player_speed * 2) : player_speed;
        correndo = IsKeyDown(KEY_LEFT_SHIFT);
        movendo  = movendo || !correndo;
    }

    // ── Resolve colisão no eixo X ─────────────────────────────────────────────
    self->position.x += vel_x;
    Rectangle hitbox = {
        self->position.x + HITBOX_OFFSET_X,
        self->position.y + HITBOX_OFFSET_Y,
        HITBOX_WIDTH,
        HITBOX_HEIGHT
    };
    for (int i = 0; i < colisor_count; i++) {
        CollisionResolve(&hitbox, colisores[i].rect);
    }
    self->position.x = hitbox.x - HITBOX_OFFSET_X;

    // ── Resolve colisão no eixo Y ─────────────────────────────────────────────
    self->position.y += vel_y;
    hitbox = (Rectangle){
        self->position.x + HITBOX_OFFSET_X,
        self->position.y + HITBOX_OFFSET_Y,
        HITBOX_WIDTH,
        HITBOX_HEIGHT
    };
    for (int i = 0; i < colisor_count; i++) {
        CollisionResolve(&hitbox, colisores[i].rect);
    }
    self->position.y = hitbox.y - HITBOX_OFFSET_Y;

    // ── Atualiza animação ─────────────────────────────────────────────────────
    if (correndo) {
        self->action_atual = run;
    } else if (movendo) {
        self->action_atual = walk;
    } else {
        self->action_atual = idle;
    }

    AnimationUpdate(&self->anims[self->action_atual]);
}

void PlayerDraw(PlayerBody *self){
    AnimationBody *anim = &self->anims[self->action_atual];
    Texture2D texture =  self->textures[self->action_atual];

    Rectangle frame = AnimationFrame(anim);
    frame.width *= self->direction;

    DrawTexturePro(
        texture,
        frame,
        (Rectangle){ self->position.x, self->position.y, PLAYER_BASE_WIDTH * PLAYER_BASE_SCALE, PLAYER_BASE_HEIGHT * PLAYER_BASE_SCALE },
        (Vector2)  { 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}

void PlayerUnload(PlayerBody *self){
    for (int i = 0; i < total_actions; i++){
        UnloadTexture(self->textures[i]);
    }
}