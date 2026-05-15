#include "renderer.h"       // ← sem "../renderer/", já está dentro da pasta
#include "raylib.h"         // ← sem "../include/", raylib.h vai pelo -I na compilação
#include "../map/map.h"
#include "../player/player.h"
#include <math.h>

#define SCREEN_W 800
#define SCREEN_H 450

void Renderer_scene( Map *m, Player *p, Texture2D *textura_parede) {  // ← nome igual ao .h, parâmetros em ordem

    DrawRectangle(0, SCREEN_H / 2, SCREEN_W, SCREEN_H / 2, DARKGRAY);

    for (int x = 0; x < SCREEN_W; x++) {
        float cameraX  = 2 * x / (float)SCREEN_W - 1;
        float rayDirX  = p->dir.x + p->plane.x * cameraX;
        float rayDirY  = p->dir.y + p->plane.y * cameraX;

        int mapX = (int)p->pos.x;
        int mapY = (int)p->pos.y;

        float deltaDistX = (rayDirX == 0) ? 1e30f : fabsf(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30f : fabsf(1.0f / rayDirY);

        float sideDistX, sideDistY;
        int stepX, stepY;

        if (rayDirX < 0) { stepX = -1; sideDistX = (p->pos.x - mapX) * deltaDistX; }
        else             { stepX =  1; sideDistX = (mapX + 1.0f - p->pos.x) * deltaDistX; }
        if (rayDirY < 0) { stepY = -1; sideDistY = (p->pos.y - mapY) * deltaDistY; }
        else             { stepY =  1; sideDistY = (mapY + 1.0f - p->pos.y) * deltaDistY; }

        int hit = 0, side = 0;
        while (hit == 0) {
            if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
            else                       { sideDistY += deltaDistY; mapY += stepY; side = 1; }
            if (Map_getgrid(m, mapX, mapY) > 0) hit = 1;  // ← era Map_getgrid
        }

        float perpWallDist = (side == 0) ? (sideDistX - deltaDistX) : (sideDistY - deltaDistY);
        int lineHeight = (int)(SCREEN_H / perpWallDist);
        int drawStart  = SCREEN_H / 2 - lineHeight / 2; if (drawStart < 0)         drawStart = 0;
        int drawEnd    = SCREEN_H / 2 + lineHeight / 2; if (drawEnd >= SCREEN_H)   drawEnd   = SCREEN_H - 1;

        float intensity = 1.0f / (1.0f + perpWallDist * perpWallDist * 0.1f);
        if (side == 1) intensity *= 0.6f;
        Color tint = (Color){ 255 * intensity, 255 * intensity, 255 * intensity, 255 };

        float wallX;
        if (side == 0) wallX = p->pos.y + perpWallDist * rayDirY;
        else           wallX = p->pos.x + perpWallDist * rayDirX;
        wallX -= floorf(wallX);

        int texX = (int)(wallX * textura_parede->width);
        if (side == 0 && rayDirX > 0) texX = textura_parede->width - texX - 1;
        if (side == 1 && rayDirY < 0) texX = textura_parede->width - texX - 1;

        Rectangle source = { texX, 0, 1, textura_parede->height };
        Rectangle dest = { x, drawStart, 1, drawEnd - drawStart + 1 };

        DrawTexturePro(*textura_parede, source, dest, (Vector2){ 0, 0 }, 0.0f, tint);
    }
}
