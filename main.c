#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdio.h>

// Configurações do Mundo e Janela
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define MAP_WIDTH 600
#define MAP_HEIGHT 600

// Mapa Estático (0 = vazio, 1-3 = paredes)
const int worldMap[MAP_WIDTH][MAP_HEIGHT] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,3,3,0,0,0,0,0,0,0,0,0,1},
    {1,0,2,2,0,0,0,0,3,3,0,1,0,3,3,3,3,0,3,3,2,0,0,1},
    {1,0,2,0,0,0,0,0,0,3,0,1,0,3,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1},
    {1,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,2,2,0,0,0,2,0,1},
    {1,0,3,0,0,0,0,0,0,2,0,1,0,0,2,0,0,0,0,2,0,0,0,1},
    {1,0,3,3,0,0,0,0,2,2,0,1,0,0,2,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,2,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int main(void) {
    // Inicialização da Janela conforme rcore
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Masmorrearemos");

    // Configuração da Câmera (Vetores)
    Vector2 pos = { 5.0f, 5.0f };      // Posição inicial
    Vector2 dir = { -1.0f, 0.0f };     // Vetor de direção
    Vector2 plane = { 0.0f, 0.66f };   // Plano da câmera (determina o FOV)

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // --- 1. Entrada de Usuário e Movimentação ---
        float deltaTime = GetFrameTime();
        float moveSpeed = deltaTime * 5.0f;
        float rotSpeed = deltaTime * 3.0f;
        //----- Movimentação Lateral --------------
        if(IsKeyDown(KEY_Q)){
            if (worldMap[(int)(pos.x - plane.x * moveSpeed)][(int)pos.y] == 0) pos.x -= plane.x * moveSpeed;
            if (worldMap[(int)pos.x][(int)(pos.y - plane.y * moveSpeed)] == 0) pos.y -= plane.y * moveSpeed;
        }  
        if(IsKeyDown(KEY_E)){
            if (worldMap[(int)(pos.x + plane.x * moveSpeed)][(int)pos.y] == 0) pos.x += plane.x * moveSpeed;
            if (worldMap[(int)pos.x][(int)(pos.y + plane.y * moveSpeed)] == 0) pos.y += plane.y * moveSpeed;
        }
        //------ Movimentação para Frente e Trás --------------
        if (IsKeyDown(KEY_W)) {
            if (worldMap[(int)(pos.x + dir.x * moveSpeed)][(int)pos.y] == 0) pos.x += dir.x * moveSpeed;
            if (worldMap[(int)pos.x][(int)(pos.y + dir.y * moveSpeed)] == 0) pos.y += dir.y * moveSpeed;
        }
                if (IsKeyDown(KEY_S)) {
            if (worldMap[(int)(pos.x - dir.x * moveSpeed)][(int)pos.y] == 0) pos.x -= dir.x * moveSpeed;
            if (worldMap[(int)pos.x][(int)(pos.y - dir.y * moveSpeed)] == 0) pos.y -= dir.y * moveSpeed;
        }
        // Rotação usando Matriz de Rotação conforme especificação
        if (IsKeyDown(KEY_D)) {
            float oldDirX = dir.x;
            dir.x = dir.x * cos(-rotSpeed) - dir.y * sin(-rotSpeed);
            dir.y = oldDirX * sin(-rotSpeed) + dir.y * cos(-rotSpeed);
            float oldPlaneX = plane.x;
            plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
            plane.y = oldPlaneX * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
        }
        if (IsKeyDown(KEY_A)) {
            float oldDirX = dir.x;
            dir.x = dir.x * cos(rotSpeed) - dir.y * sin(rotSpeed);
            dir.y = oldDirX * sin(rotSpeed) + dir.y * cos(rotSpeed);
            float oldPlaneX = plane.x;
            plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
            plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);
        }

        // --- 2. Renderização Raycasting (DDA) ---
        BeginDrawing();
        ClearBackground(BLACK);

        // Desenhar Chão e Teto (Simples)
        DrawRectangle(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2, DARKGRAY);

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            // Calcular posição e direção do raio
            float cameraX = 2 * x / (float)SCREEN_WIDTH - 1; 
            float rayDirX = dir.x + plane.x * cameraX;
            float rayDirY = dir.y + plane.y * cameraX;

            // Posição na grade do mapa
            int mapX = (int)pos.x;
            int mapY = (int)pos.y;

            // Comprimento do raio de uma linha da grade para a próxima
            float deltaDistX = (rayDirX == 0) ? 1e30 : fabsf(1 / rayDirX);
            float deltaDistY = (rayDirY == 0) ? 1e30 : fabsf(1 / rayDirY);
            
            float sideDistX, sideDistY;
            int stepX, stepY;
            int hit = 0; 
            int side; // 0 para X, 1 para Y

            // Passo inicial e distância lateral
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (pos.x - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0f - pos.x) * deltaDistX;
            }
            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (pos.y - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0f - pos.y) * deltaDistY;
            }

            // --- Loop DDA ---
            while (hit == 0) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                if (worldMap[mapX][mapY] > 0) hit = 1;
            }

            // Distância perpendicular (evita fisheye)
            float perpWallDist;
            if (side == 0) perpWallDist = (sideDistX - deltaDistX);
            else           perpWallDist = (sideDistY - deltaDistY);

            // Calcular altura da linha na tela
            int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

            // Calcular pixels de início e fim da coluna
            int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
            if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

            // Definir cor baseada no ID do mapa e lado
            Color color = RED;
            if (worldMap[mapX][mapY] == 2) color = GREEN;
            if (worldMap[mapX][mapY] == 3) color = BLUE;
            if (side == 1) color = (Color){ color.r/2, color.g/2, color.b/2, 255 }; // Sombra

            float intensity = 1.0f / (1.0f + perpWallDist * perpWallDist * 0.1f);
            Color tint = (Color){ 255 * intensity, 255 * intensity, 255 * intensity, 255 };
            // Desenhar a coluna vertical
            DrawLine(x, drawStart, x, drawEnd, color);
        }

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}