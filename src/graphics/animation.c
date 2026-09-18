#include "graphics/animation.h"

void AnimationUpdate(Animation *self){
    float delta_time = GetFrameTime(); // Tempo em segundos que o último quadro demorou pra renderizar.
    self->duration_left -= delta_time; 

    if (self->duration_left <= 0){
        self->duration_left = self->speed;
        self->frame_atual_idx += self->step;

        if (self->frame_atual_idx > self->last_idx){
            //Animação normal.
            switch (self->type){
                case REPEATING:
                    self->frame_atual_idx = self->first_idx;
                    break;
                
                case ONESHOT:
                    self->frame_atual_idx = self->last_idx;
                    break;

                default:
                    break;
            }
        }else if (self->frame_atual_idx < self->first_idx){
            // Animação reversa.
            switch (self->type){
                case REPEATING:
                    self->frame_atual_idx = self->last_idx;
                    break;
                
                case ONESHOT:
                    self->frame_atual_idx = self->first_idx;
                    break;

                default:
                    break;
            }
        }
    }
}

Rectangle AnimationFrame(Animation *self, int num_frames_por_linha, float spr_dimension){
    int x = (self->frame_atual_idx % num_frames_por_linha) * spr_dimension; //Cálculo da coluna (eixo x)
    int y = (self->frame_atual_idx / num_frames_por_linha) * spr_dimension; //Cálculo da linha (eixo y)

    return (Rectangle){.x = (float)x, .y = (float)y, .width = spr_dimension, .height = spr_dimension};
}