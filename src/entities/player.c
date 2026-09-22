#include <raylib.h>
#include "entities/player.h"

//----------- Funções internas (não expostas no .h) - Essas são de uso exclusivo desse arquivo .c

//Inicializa as animações carregando as texturas e montando os corpos dos tipos de animações do player.
static void InitAnimations(PlayerBody *self){
    self->textures[idle] = LoadTexture("assets/texture/sprites_teste/tiny_heroes_spr/3 Dude_Monster/Dude_Monster_Idle_4.png");
    self->anims[idle] = (AnimationBody){
            .first_idx = 0,
            .last_idx = 3,
            .frame_atual_idx = 0,
            .step = 1,
            .speed = 0.1,
            .duration_left = 0.1,
            .frames_por_linha = 4,
            .spr_dimension = 32.0,
            .type = REPEATING
        };

    self->textures[walk] = LoadTexture("assets/texture/sprites_teste/tiny_heroes_spr/3 Dude_Monster/Dude_Monster_Walk_6.png");
    self->anims[walk] = (AnimationBody){
            .first_idx = 0,
            .last_idx = 5,
            .frame_atual_idx = 0,
            .step = 1,
            .speed = 0.1,
            .duration_left = 0.1,
            .frames_por_linha = 5,
            .spr_dimension = 32.0,
            .type = REPEATING
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
            .spr_dimension = 32.0,            
            .type = REPEATING
        };
}

//Reseta o quadro de todas as animações do player para o estado inicial delas.
void ResetAnimations(PlayerBody *self){
    for (int i = 0; i < self->action_atual; i++){
        self->anims[i].frame_atual_idx = self->anims[i].first_idx;
        self->anims[i].duration_left = self->anims[i].speed;
    }
}

// -------- Funções visiveis para qualquer lugar que importalas.

void PlayerInit(PlayerBody *self){
    InitAnimations(self);

    self->position = (Vector2){ 100.0f, 100.0f };
    self->direction = RIGHT;
    self->action_atual = idle;
}

void PLayerReset(PlayerBody *self){
    //Texturas já estão carregadas, só reseta estado.
    ResetAnimations(self);

    self->position     = (Vector2){ 100.0f, 100.0f };
    self->direction    = RIGHT;
    self->action_atual = idle;
}

void PlayerUpdateAction(PlayerBody *self){
    float player_speed = 4.0f;

    bool movendo = false;
    bool correndo = false;

    //Eixo X (direita e esquerda são opostos — não podem ser simultâneos).
    if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            self->direction   = LEFT;
            self->position.x -= (player_speed * 2);
            correndo = true;
        }else {
            self->direction   = LEFT;
            self->position.x -= player_speed;
            movendo = true;
        }
        
    } else if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT)) {

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            self->direction   = RIGHT;
            self->position.x += (player_speed * 2);
            correndo = true;
        }else {
            self->direction   = RIGHT;
            self->position.x += player_speed;
            movendo = true;
        }

    }

    //Eixo Y (cima e baixo são opostos — não podem ser simultâneos).
    if (IsKeyDown(KEY_UP) && !IsKeyDown(KEY_DOWN)) {

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            self->position.y -= (player_speed * 2);
            correndo = true;
        }else {
            self->position.y -= player_speed;
            movendo = true;
        }

    } else if (IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP)) {

        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            self->position.y += (player_speed * 2);
            correndo = true;
        }else {
            self->position.y += player_speed;
            movendo = true;
        }

    }

    //Tipo de animação gerada.
    if (movendo) {
        self->action_atual = walk;
    }else if (correndo) {
        self->action_atual = run;
    } else {
        self->action_atual = idle;
    }

    //Atualiza só a animação ativa.
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
        (Rectangle){ self->position.x, self->position.y, 128.0f, 128.0f },
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