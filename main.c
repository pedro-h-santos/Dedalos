#include "raylib.h"
#include <math.h>

#define MAP_SIZE 8
#define TILE_SIZE 64

int map[MAP_SIZE][MAP_SIZE] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1},
};

int main() {
    InitWindow(1200, 600, "Raycasting - Do 2D para o 3D");
    
    Vector2 playerPos = { 150, 150 };
    float playerAngle = 0.0f; // Direção que o jogador olha

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Rotação e Movimento
        if (IsKeyDown(KEY_LEFT))  playerAngle -= 0.05f;
        if (IsKeyDown(KEY_RIGHT)) playerAngle += 0.05f;
        if (IsKeyDown(KEY_UP)) {
            playerPos.x += cos(playerAngle) * 3;
            playerPos.y += sin(playerAngle) * 3;
        }
        if (IsKeyDown(KEY_DOWN)) {
            playerPos.x -= cos(playerAngle) * 3;
            playerPos.y -= sin(playerAngle) * 3;
        }

        BeginDrawing();
            ClearBackground(BLACK);

            // --- PARTE 1: VISÃO 2D (Lado Esquerdo) ---
            for (int y = 0; y < MAP_SIZE; y++) {
                for (int x = 0; x < MAP_SIZE; x++) {
                    if (map[y][x] == 1) DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE-1, TILE_SIZE-1, DARKGRAY);
                }
            }

            // Lançando raios (FOV de 60 graus)
            for (int i = 0; i < 60; i++) {
                float rayAngle = (playerAngle - 0.5f) + ((float)i / 60.0f);
                float distance = 0;
                float x = playerPos.x;
                float y = playerPos.y;

                // Faz o raio caminhar até bater na parede
                while (map[(int)(y / TILE_SIZE)][(int)(x / TILE_SIZE)] == 0 && distance < 600) {
                    distance += 1;
                    x = playerPos.x + cos(rayAngle) * distance;
                    y = playerPos.y + sin(rayAngle) * distance;
                }

                // Desenha o raio no 2D
                DrawLineV(playerPos, (Vector2){x, y}, YELLOW);

                // --- PARTE 2: PROJEÇÃO 3D (Lado Direito) ---
                // Quanto menor a distância, maior a coluna
                float wallHeight = (TILE_SIZE * 600) / distance; 
                if (wallHeight > 600) wallHeight = 600;

                // Desenha a coluna vertical no lado direito da tela (offset de 600px)
                DrawRectangle(600 + (i * 10), (600 - wallHeight) / 2, 9, wallHeight, BLUE);
            }

            DrawCircleV(playerPos, 5, RED); // Jogador
        EndDrawing();
    }
    CloseWindow();
    return 0;
}