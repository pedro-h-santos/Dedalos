#include "player.h"
#include "../map/map.h"
#include <math.h>

void Player_init(Player *p, float x, float y) {
    p->pos.x   = x;
    p->pos.y   = y;
    p->dir.x   = -1.0f;
    p->dir.y   =  0.0f;
    p->plane.x =  0.0f;
    p->plane.y =  0.66f;
}

void Player_handle_input(Player *p, Map *m, float dt) {
    float moveSpeed = 5.0f * dt;
    float rotSpeed  = 3.0f * dt;

    if (IsKeyDown(KEY_Q)) {
        if (Map_is_walkable(m, p->pos.x - p->plane.x * moveSpeed, p->pos.y)) p->pos.x -= p->plane.x * moveSpeed;
        if (Map_is_walkable(m, p->pos.x, p->pos.y - p->plane.y * moveSpeed)) p->pos.y -= p->plane.y * moveSpeed;
    }
    if (IsKeyDown(KEY_E)) {
        if (Map_is_walkable(m, p->pos.x + p->plane.x * moveSpeed, p->pos.y)) p->pos.x += p->plane.x * moveSpeed;
        if (Map_is_walkable(m, p->pos.x, p->pos.y + p->plane.y * moveSpeed)) p->pos.y += p->plane.y * moveSpeed;
    }
    if (IsKeyDown(KEY_W)) {
        if (Map_is_walkable(m, p->pos.x + p->dir.x * moveSpeed, p->pos.y)) p->pos.x += p->dir.x * moveSpeed;
        if (Map_is_walkable(m, p->pos.x, p->pos.y + p->dir.y * moveSpeed)) p->pos.y += p->dir.y * moveSpeed;
    }
    if (IsKeyDown(KEY_S)) {
        if (Map_is_walkable(m, p->pos.x - p->dir.x * moveSpeed, p->pos.y)) p->pos.x -= p->dir.x * moveSpeed;
        if (Map_is_walkable(m, p->pos.x, p->pos.y - p->dir.y * moveSpeed)) p->pos.y -= p->dir.y * moveSpeed;
    }

    if (IsKeyDown(KEY_D)) {
        float c = cosf(-rotSpeed), s = sinf(-rotSpeed);
        float oldDirX = p->dir.x;
        p->dir.x = p->dir.x * c - p->dir.y * s;
        p->dir.y = oldDirX * s + p->dir.y * c;

        float oldPlX = p->plane.x;
        p->plane.x = p->plane.x * c - p->plane.y * s;
        p->plane.y = oldPlX * s + p->plane.y * c;
    }
    if (IsKeyDown(KEY_A)) {
        float c = cosf(rotSpeed), s = sinf(rotSpeed);
        float oldDirX = p->dir.x;
        p->dir.x = p->dir.x * c - p->dir.y * s;
        p->dir.y = oldDirX * s + p->dir.y * c;

        float oldPlX = p->plane.x;
        p->plane.x = p->plane.x * c - p->plane.y * s;
        p->plane.y = oldPlX * s + p->plane.y * c;
    }
}
