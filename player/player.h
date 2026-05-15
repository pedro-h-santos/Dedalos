#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "../map/map.h"


typedef struct {
    Vector2 pos;   // Posição do jogador
    Vector2 dir;   // Direção que o jogador está olhando
    Vector2 plane; // Plano da câmera para projeção 3D
} Player;

void Player_init(Player *p, float x, float y);
void Player_handle_input(Player *p, Map *m, float dt);

#endif
