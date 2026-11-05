/**
 * @file resources.c
 * @brief Implementação do carregamento de recursos globais.
 */

#include "core/resources.h"

void ResourcesInit(ResourcesBody *self) {
    self->fonte_principal = LoadFontEx("assets/fonts/VT323/VT323-Regular.ttf", 64, 0, 0);
    self->ranking_count   = 0;
    DataLoad(self->ranking, &self->ranking_count);
}

void ResourcesUnload(ResourcesBody *self) {
    UnloadFont(self->fonte_principal);
}