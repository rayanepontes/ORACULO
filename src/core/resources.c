/**
 * @file resources.c
 * @brief Implementação do carregamento de recursos globais.
 */

#include "core/resources.h"

void ResourcesInit(ResourcesBody *self) {
    self->fonte_principal = LoadFontEx("assets/fonts/VT323/VT323-Regular.ttf", 64, 0, 0);
    self->ranking_count   = 0;
    DataLoad(self->ranking, &self->ranking_count);
    self->amarelo      = GetColor(0xEFB239FF);
    self->branco       = GetColor(0xE5E0EBFF);
    self->lilas_escuro = GetColor(0x6B5E8CFF);
    self->lilas_hint   = GetColor(0x9988BBFF);
    self->letra_caixa  = GetColor(0x2A1F4EFF);
}

void ResourcesUnload(ResourcesBody *self) {
    UnloadFont(self->fonte_principal);
}