/** @file map.c
 * @brief Implementação do módulo de mapa.
 */
#include "map/map.h"

// ─── API pública ──────────────────────────────────────────────────────────────
void MapInitCollision(MapBody *self){
    // colisores — defina os retângulos olhando o mapa com CollisionDrawDebug
    self->colisor_count = 0;

    // exemplo de como adicionar quando for testar:
    // self->colisores[self->colisor_count++] = (CollisionRect){
    //     .rect = { x1, y1, x2-x1, y2-y1 },  // onde x1 e y1 são a ponta superior esquerda do retângulo
    //                                           e x2 e y2 a ponta inferior direta
    //     .tag  = "borda_esq"
    // };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 50*MAP_ESCALA, 300*MAP_ESCALA, 720*MAP_ESCALA, 250*MAP_ESCALA },
    .tag  = "sala_1_parede_cima"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 45*MAP_ESCALA, 310*MAP_ESCALA, 28*MAP_ESCALA, 700*MAP_ESCALA },
    .tag  = "sala_1_parede_esquerda"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 748*MAP_ESCALA, 310*MAP_ESCALA, 28*MAP_ESCALA, 700*MAP_ESCALA },
    .tag  = "sala_1_parede_direita"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 72*MAP_ESCALA, 1011*MAP_ESCALA, 159*MAP_ESCALA, 32*MAP_ESCALA },
    .tag  = "sala_1_parede_baixo_esquerda"
    };
    
    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 362*MAP_ESCALA, 1011*MAP_ESCALA, 386*MAP_ESCALA, 32*MAP_ESCALA },
    .tag  = "sala_1_parede_baixo_direita"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1020*MAP_ESCALA, 48*MAP_ESCALA, 721*MAP_ESCALA, 258*MAP_ESCALA },
    .tag  = "sala_2_parede_cima"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1015*MAP_ESCALA, 50*MAP_ESCALA, 13*MAP_ESCALA, 740*MAP_ESCALA },
    .tag  = "sala_2_parede_esquerda"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1026*MAP_ESCALA, 790*MAP_ESCALA, 774*MAP_ESCALA, 28*MAP_ESCALA },
    .tag  = "sala_2_parede_baixo"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1800*MAP_ESCALA, 766*MAP_ESCALA, 26*MAP_ESCALA, 24*MAP_ESCALA },
    .tag  = "sala_2_parede_baixo"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 3044*MAP_ESCALA, 261*MAP_ESCALA, 704*MAP_ESCALA, 239*MAP_ESCALA },
    .tag  = "sala_3_parede_cima"
    };    

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 2971*MAP_ESCALA, 779*MAP_ESCALA, 22*MAP_ESCALA, 305*MAP_ESCALA },
    .tag  = "sala_3_parede_esquerda"
    };  
    
    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 3803*MAP_ESCALA, 365*MAP_ESCALA, 23*MAP_ESCALA, 718*MAP_ESCALA },
    .tag  = "sala_3_parede_direita"
    }; 

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 3043*MAP_ESCALA, 1186*MAP_ESCALA, 711*MAP_ESCALA, 24*MAP_ESCALA },
    .tag  = "sala_3_parede_baixo"
    };  

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1330*MAP_ESCALA, 792*MAP_ESCALA, 1345*MAP_ESCALA, 330*MAP_ESCALA },
    .tag  = "sala_4_parede_cima"
    };  

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1170*MAP_ESCALA, 1631*MAP_ESCALA, 1435*MAP_ESCALA, 489*MAP_ESCALA },
    .tag  = "sala_4_parede_baixo"
    };  
   
    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 228*MAP_ESCALA, 1479*MAP_ESCALA, 645*MAP_ESCALA, 321*MAP_ESCALA },
    .tag  = "sala_5_parede_cima_1"
    };  

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1018*MAP_ESCALA, 1479*MAP_ESCALA, 149*MAP_ESCALA, 318*MAP_ESCALA },
    .tag  = "sala_5_parede_cima_2"
    };  

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 246*MAP_ESCALA, 1856*MAP_ESCALA, 25*MAP_ESCALA, 198*MAP_ESCALA },
    .tag  = "sala_5_parede_esquerda"
    };  

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 491*MAP_ESCALA, 2128*MAP_ESCALA, 3029*MAP_ESCALA, 27*MAP_ESCALA },
    .tag  = "sala_5e6_parede_baixo"
    };  

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 193*MAP_ESCALA, 1028*MAP_ESCALA, 36*MAP_ESCALA, 451*MAP_ESCALA },
    .tag  = "corredor_1_esquerda"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 367*MAP_ESCALA, 1036*MAP_ESCALA, 887*MAP_ESCALA, 296*MAP_ESCALA },
    .tag  = "corredor_1_cima"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1177*MAP_ESCALA, 1477*MAP_ESCALA, 76*MAP_ESCALA, 68*MAP_ESCALA },
    .tag  = "corredor_1_baixo"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1177*MAP_ESCALA, 1477*MAP_ESCALA, 76*MAP_ESCALA, 68*MAP_ESCALA },
    .tag  = "corredor_1_baixo"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 3502*MAP_ESCALA, 1626*MAP_ESCALA, 22*MAP_ESCALA, 499*MAP_ESCALA },
    .tag  = "sala_6_parede_direita"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 2476*MAP_ESCALA, 1533*MAP_ESCALA, 287*MAP_ESCALA, 260*MAP_ESCALA },
    .tag  = "sala_6_parede_cima_1"
    };    

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 2897*MAP_ESCALA, 1533*MAP_ESCALA, 383*MAP_ESCALA, 260*MAP_ESCALA },
    .tag  = "sala_6_parede_cima_2"
    };   
    
    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 2482*MAP_ESCALA, 1132*MAP_ESCALA, 193*MAP_ESCALA, 254*MAP_ESCALA },
    .tag  = "corredor_2_esquerda"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 2896*MAP_ESCALA, 794*MAP_ESCALA, 97*MAP_ESCALA, 747*MAP_ESCALA },
    .tag  = "corredor_2_direita"
    };

    self->colisores[self->colisor_count++] = (CollisionRect){
    .rect = { 1795*MAP_ESCALA, 397*MAP_ESCALA, 1200*MAP_ESCALA, 223*MAP_ESCALA },
    .tag  = "corredor_3_cima"
    };

    //Inicio da parede com angulação da sala 2

    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1742*MAP_ESCALA, 293*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_1" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1746*MAP_ESCALA, 303*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_2" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1750*MAP_ESCALA, 313*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_3" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1755*MAP_ESCALA, 323*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_4" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1759*MAP_ESCALA, 333*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_5" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1763*MAP_ESCALA, 343*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_6" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1768*MAP_ESCALA, 353*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_7" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1772*MAP_ESCALA, 363*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_8" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1776*MAP_ESCALA, 373*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_9" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1780*MAP_ESCALA, 383*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_10" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1784*MAP_ESCALA, 393*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_11" };
    self->colisores[self->colisor_count++] = (CollisionRect){ .rect = { 1789*MAP_ESCALA, 403*MAP_ESCALA, 14*MAP_ESCALA, 10*MAP_ESCALA }, .tag = "diag_12" };

    //Final
    // O resto de TODAS as diagonais
    // diag_A: (3043,488) -> (2995,627), espessura=20
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3039.3f*MAP_ESCALA, 488.0f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3035.6f*MAP_ESCALA, 498.7f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3031.9f*MAP_ESCALA, 509.4f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3028.2f*MAP_ESCALA, 520.1f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3024.5f*MAP_ESCALA, 530.8f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3020.8f*MAP_ESCALA, 541.5f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3017.2f*MAP_ESCALA, 552.2f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3013.5f*MAP_ESCALA, 562.8f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3009.8f*MAP_ESCALA, 573.5f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3006.1f*MAP_ESCALA, 584.2f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3002.4f*MAP_ESCALA, 594.9f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_10"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2998.7f*MAP_ESCALA, 605.6f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_11"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2995.0f*MAP_ESCALA, 616.3f*MAP_ESCALA, 23.7f*MAP_ESCALA, 30.7f*MAP_ESCALA },
        .tag  = "diag_A_12"
    };
 
    // diag_B: (3752,488) -> (3804,603), espessura=16
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3752.0f*MAP_ESCALA, 488.0f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3756.7f*MAP_ESCALA, 498.5f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3761.5f*MAP_ESCALA, 508.9f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3766.2f*MAP_ESCALA, 519.4f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3770.9f*MAP_ESCALA, 529.8f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3775.6f*MAP_ESCALA, 540.3f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3780.4f*MAP_ESCALA, 550.7f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3785.1f*MAP_ESCALA, 561.2f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3789.8f*MAP_ESCALA, 571.6f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3794.5f*MAP_ESCALA, 582.1f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3799.3f*MAP_ESCALA, 592.5f*MAP_ESCALA, 20.7f*MAP_ESCALA, 26.5f*MAP_ESCALA },
        .tag  = "diag_B_10"
    };
 
    // diag_C: (2993,1085) -> (3028,1208), espessura=26
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2993.0f*MAP_ESCALA, 1085.0f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2995.9f*MAP_ESCALA, 1095.2f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2998.8f*MAP_ESCALA, 1105.5f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3001.8f*MAP_ESCALA, 1115.8f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3004.7f*MAP_ESCALA, 1126.0f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3007.6f*MAP_ESCALA, 1136.2f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3010.5f*MAP_ESCALA, 1146.5f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3013.4f*MAP_ESCALA, 1156.8f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3016.3f*MAP_ESCALA, 1167.0f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3019.2f*MAP_ESCALA, 1177.2f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3022.2f*MAP_ESCALA, 1187.5f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_10"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3025.1f*MAP_ESCALA, 1197.8f*MAP_ESCALA, 28.9f*MAP_ESCALA, 36.2f*MAP_ESCALA },
        .tag  = "diag_C_11"
    };
 
    // diag_D: (3803,1086) -> (3775,1199), espessura=26
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3800.5f*MAP_ESCALA, 1086.0f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3797.9f*MAP_ESCALA, 1096.3f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3795.4f*MAP_ESCALA, 1106.5f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3792.8f*MAP_ESCALA, 1116.8f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3790.3f*MAP_ESCALA, 1127.1f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3787.7f*MAP_ESCALA, 1137.4f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3785.2f*MAP_ESCALA, 1147.6f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3782.6f*MAP_ESCALA, 1157.9f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3780.1f*MAP_ESCALA, 1168.2f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3777.5f*MAP_ESCALA, 1178.5f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3775.0f*MAP_ESCALA, 1188.7f*MAP_ESCALA, 28.5f*MAP_ESCALA, 36.3f*MAP_ESCALA },
        .tag  = "diag_D_10"
    };
 
    // diag_E: (1330,1147) -> (1254,1270), espessura=20
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1323.7f*MAP_ESCALA, 1147.0f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1317.3f*MAP_ESCALA, 1157.2f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1311.0f*MAP_ESCALA, 1167.5f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1304.7f*MAP_ESCALA, 1177.8f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1298.3f*MAP_ESCALA, 1188.0f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1292.0f*MAP_ESCALA, 1198.2f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1285.7f*MAP_ESCALA, 1208.5f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1279.3f*MAP_ESCALA, 1218.8f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1273.0f*MAP_ESCALA, 1229.0f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1266.7f*MAP_ESCALA, 1239.2f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1260.3f*MAP_ESCALA, 1249.5f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_10"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1254.0f*MAP_ESCALA, 1259.8f*MAP_ESCALA, 26.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_E_11"
    };
 
    // diag_F: (2420,1149) -> (2478,1261), espessura=20
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2420.0f*MAP_ESCALA, 1149.0f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2425.3f*MAP_ESCALA, 1159.2f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2430.5f*MAP_ESCALA, 1169.4f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2435.8f*MAP_ESCALA, 1179.5f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2441.1f*MAP_ESCALA, 1189.7f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2446.4f*MAP_ESCALA, 1199.9f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2451.6f*MAP_ESCALA, 1210.1f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2456.9f*MAP_ESCALA, 1220.3f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2462.2f*MAP_ESCALA, 1230.5f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2467.5f*MAP_ESCALA, 1240.6f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2472.7f*MAP_ESCALA, 1250.8f*MAP_ESCALA, 25.3f*MAP_ESCALA, 30.2f*MAP_ESCALA },
        .tag  = "diag_F_10"
    };
 
    // diag_G: (1252,1546) -> (1311,1655), espessura=29
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1252.0f*MAP_ESCALA, 1546.0f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1257.9f*MAP_ESCALA, 1556.9f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1263.8f*MAP_ESCALA, 1567.8f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1269.7f*MAP_ESCALA, 1578.7f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1275.6f*MAP_ESCALA, 1589.6f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1281.5f*MAP_ESCALA, 1600.5f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1287.4f*MAP_ESCALA, 1611.4f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1293.3f*MAP_ESCALA, 1622.3f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1299.2f*MAP_ESCALA, 1633.2f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 1305.1f*MAP_ESCALA, 1644.1f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.9f*MAP_ESCALA },
        .tag  = "diag_G_9"
    };
 
    // diag_H: (2475,1552) -> (2416,1659), espessura=29
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2469.1f*MAP_ESCALA, 1552.0f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2463.2f*MAP_ESCALA, 1562.7f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2457.3f*MAP_ESCALA, 1573.4f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2451.4f*MAP_ESCALA, 1584.1f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2445.5f*MAP_ESCALA, 1594.8f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2439.6f*MAP_ESCALA, 1605.5f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2433.7f*MAP_ESCALA, 1616.2f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2427.8f*MAP_ESCALA, 1626.9f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2421.9f*MAP_ESCALA, 1637.6f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 2416.0f*MAP_ESCALA, 1648.3f*MAP_ESCALA, 34.9f*MAP_ESCALA, 39.7f*MAP_ESCALA },
        .tag  = "diag_H_9"
    };
 
    // diag_I: (271,1854) -> (492,1797), espessura=14
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 271.0f*MAP_ESCALA, 1851.4f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 281.0f*MAP_ESCALA, 1848.8f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 291.1f*MAP_ESCALA, 1846.2f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 301.1f*MAP_ESCALA, 1843.6f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 311.2f*MAP_ESCALA, 1841.0f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 321.2f*MAP_ESCALA, 1838.5f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 331.3f*MAP_ESCALA, 1835.9f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 341.3f*MAP_ESCALA, 1833.3f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 351.4f*MAP_ESCALA, 1830.7f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 361.4f*MAP_ESCALA, 1828.1f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 371.5f*MAP_ESCALA, 1825.5f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_10"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 381.5f*MAP_ESCALA, 1822.9f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_11"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 391.5f*MAP_ESCALA, 1820.3f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_12"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 401.6f*MAP_ESCALA, 1817.7f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_13"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 411.6f*MAP_ESCALA, 1815.1f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_14"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 421.7f*MAP_ESCALA, 1812.5f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_15"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 431.7f*MAP_ESCALA, 1810.0f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_16"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 441.8f*MAP_ESCALA, 1807.4f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_17"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 451.8f*MAP_ESCALA, 1804.8f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_18"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 461.9f*MAP_ESCALA, 1802.2f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_19"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 471.9f*MAP_ESCALA, 1799.6f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_20"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 482.0f*MAP_ESCALA, 1797.0f*MAP_ESCALA, 24.0f*MAP_ESCALA, 16.6f*MAP_ESCALA },
        .tag  = "diag_I_21"
    };
 
    // diag_J: (272,2055) -> (480,2158), espessura=29
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 272.0f*MAP_ESCALA, 2055.0f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 282.4f*MAP_ESCALA, 2060.2f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 292.8f*MAP_ESCALA, 2065.3f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.1f*MAP_ESCALA },
        .tag  = "diag_J_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 303.2f*MAP_ESCALA, 2070.4f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 313.6f*MAP_ESCALA, 2075.6f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 324.0f*MAP_ESCALA, 2080.8f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 334.4f*MAP_ESCALA, 2085.9f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 344.8f*MAP_ESCALA, 2091.1f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.1f*MAP_ESCALA },
        .tag  = "diag_J_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 355.2f*MAP_ESCALA, 2096.2f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 365.6f*MAP_ESCALA, 2101.3f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 376.0f*MAP_ESCALA, 2106.5f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_10"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 386.4f*MAP_ESCALA, 2111.7f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_11"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 396.8f*MAP_ESCALA, 2116.8f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.1f*MAP_ESCALA },
        .tag  = "diag_J_12"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 407.2f*MAP_ESCALA, 2121.9f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_13"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 417.6f*MAP_ESCALA, 2127.1f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_14"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 428.0f*MAP_ESCALA, 2132.2f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_15"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 438.4f*MAP_ESCALA, 2137.4f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_16"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 448.8f*MAP_ESCALA, 2142.6f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.1f*MAP_ESCALA },
        .tag  = "diag_J_17"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 459.2f*MAP_ESCALA, 2147.7f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_18"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 469.6f*MAP_ESCALA, 2152.8f*MAP_ESCALA, 39.4f*MAP_ESCALA, 34.2f*MAP_ESCALA },
        .tag  = "diag_J_19"
    };
 
    // diag_K: (3281,1778) -> (3499,1862), espessura=14
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3281.0f*MAP_ESCALA, 1778.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_0"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3291.4f*MAP_ESCALA, 1782.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_1"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3301.8f*MAP_ESCALA, 1786.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_2"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3312.1f*MAP_ESCALA, 1790.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_3"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3322.5f*MAP_ESCALA, 1794.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_4"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3332.9f*MAP_ESCALA, 1798.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_5"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3343.3f*MAP_ESCALA, 1802.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_6"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3353.7f*MAP_ESCALA, 1806.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_7"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3364.0f*MAP_ESCALA, 1810.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_8"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3374.4f*MAP_ESCALA, 1814.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_9"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3384.8f*MAP_ESCALA, 1818.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_10"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3395.2f*MAP_ESCALA, 1822.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_11"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3405.6f*MAP_ESCALA, 1826.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_12"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3416.0f*MAP_ESCALA, 1830.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_13"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3426.3f*MAP_ESCALA, 1834.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_14"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3436.7f*MAP_ESCALA, 1838.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_15"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3447.1f*MAP_ESCALA, 1842.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_16"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3457.5f*MAP_ESCALA, 1846.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_17"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3467.9f*MAP_ESCALA, 1850.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_18"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3478.2f*MAP_ESCALA, 1854.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_19"
    };
    self->colisores[self->colisor_count++] = (CollisionRect){
        .rect = { 3488.6f*MAP_ESCALA, 1858.0f*MAP_ESCALA, 24.4f*MAP_ESCALA, 18.0f*MAP_ESCALA },
        .tag  = "diag_K_20"
    };
    //Acabou...
}

void MapInit(MapBody *self, MapType type, const char *texture_path) {
    self->type          = type;
    self->texture       = LoadTexture(texture_path);
    self->crop.source   = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
    self->crop.dest     = (Rectangle){ 0.0f, 0.0f, 0.0f, 0.0f };
    MapInitCollision(self);
}

void MapSetCrop(MapBody *self, Rectangle source, Rectangle dest) {
    self->crop.source = source;
    self->crop.dest   = dest;
}

void MapDraw(MapBody *self) {
    switch (self->type) {

        case MAP_SINGLE:
            DrawTextureEx(self->texture, (Vector2){0, 0}, 0.0f, (float)MAP_ESCALA, WHITE);
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