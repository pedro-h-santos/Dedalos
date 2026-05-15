#ifndef RENDERER_H
#define RENDERER_H

#include "../map/map.h"
#include "../player/player.h"
#include "raylib.h"

void Renderer_scene(Map *m, Player *p, Texture2D *textura_parede);
#endif
