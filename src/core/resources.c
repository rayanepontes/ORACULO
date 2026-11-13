/**
 * @file resources.c
 * @brief Implementação do carregamento de recursos globais.
 */

#include "core/resources.h"

void ResourcesInit(ResourcesBody *self) {
    self->fonte_principal = LoadFontEx(RESOURCES_FONTE_PATH, RESOURCES_FONTE_SIZE, 0, 0);
    self->ranking_count   = 0;
    DataLoad(self->ranking, &self->ranking_count);
    self->amarelo      = GetColor(0xEFB239FF);
    self->branco       = GetColor(0xE5E0EBFF);
    self->azul_escuro  = GetColor(0x000F19FF);
    self->lilas_escuro = GetColor(0x6B5E8CFF);
    self->lilas_claro  = GetColor(0x9988BBFF);
    self->ciano_claro  = GetColor(0xAEFFFAFF);
    self->letra_caixa  = GetColor(0x2A1F4EFF);
    self->musica_menu     = LoadMusicStream(RESOURCES_MUSICA_MENU_PATH);
    self->musica_gameplay = LoadMusicStream(RESOURCES_MUSICA_GAMEPLAY_PATH);
    self->musica_menu.looping     = true;
    self->musica_gameplay.looping = true;
    SetMusicVolume(self->musica_menu,     RESOURCES_MUSICA_VOLUME);
    SetMusicVolume(self->musica_gameplay, RESOURCES_MUSICA_VOLUME);
}

void ResourcesUnload(ResourcesBody *self) {
    UnloadFont(self->fonte_principal);
    UnloadMusicStream(self->musica_menu);
    UnloadMusicStream(self->musica_gameplay);
}