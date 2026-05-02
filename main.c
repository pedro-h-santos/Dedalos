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
    
    // Posicionar o jogador em um tile vazio do mapa.
    // Antes ele estava em (150,150), que cai dentro da parede em map[2][2] == 1.
    Vector2 playerPos = { 96, 96 };
    float playerAngle = 0.0f; // Direção que o jogador olha

    // Definir nível de log para ver mensagens de erro e info no console
    SetTraceLogLevel(LOG_INFO);

    // Mostrar o diretório de trabalho atual, para saber onde o carregamento relativo está sendo feito
    const char *workingDir = GetWorkingDirectory();
    TraceLog(LOG_INFO, "Diretório de trabalho: %s", workingDir);

    // Tentar carregar a textura usando vários caminhos possíveis, incluindo o caminho absoluto
    const char *wallPaths[] = {
        "C:/projetos/dedalos/img/parede.jpg",
        "C:/projetos/Dedalos/img/parede.jpg",
        "img/parede.jpg",
        ".\\img\\parede.jpg",
        "..\\img\\parede.jpg",
        "..\\..\\img\\parede.jpg"
    };
    Texture2D wallTexture = { 0 };
    const char *wallPath = wallPaths[0];
    for (int i = 0; i < sizeof(wallPaths) / sizeof(wallPaths[0]); i++) {
        wallTexture = LoadTexture(wallPaths[i]);
        if (wallTexture.id != 0) {
            wallPath = wallPaths[i];
            TraceLog(LOG_INFO, "Textura carregada com sucesso: %s", wallPath);
            break;
        }
        TraceLog(LOG_WARNING, "Falha ao carregar textura: %s", wallPaths[i]);
    }
    if (wallTexture.id == 0) {
        TraceLog(LOG_ERROR, "Falha ao carregar qualquer textura de parede.");
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Rotação e Movimento com colisão
        if (IsKeyDown(KEY_LEFT))  playerAngle -= 0.05f;
        if (IsKeyDown(KEY_RIGHT)) playerAngle += 0.05f;
        if (IsKeyDown(KEY_UP)) {
            // Calcular nova posição
            float newX = playerPos.x + cos(playerAngle) * 3;
            float newY = playerPos.y + sin(playerAngle) * 3;
            // Verificar se a nova posição não é uma parede (map == 0)
            if (map[(int)(newY / TILE_SIZE)][(int)(newX / TILE_SIZE)] == 0) {
                playerPos.x = newX;
                playerPos.y = newY;
            }
        }
        if (IsKeyDown(KEY_DOWN)) {
            // Calcular nova posição
            float newX = playerPos.x - cos(playerAngle) * 3;
            float newY = playerPos.y - sin(playerAngle) * 3;
            // Verificar se a nova posição não é uma parede (map == 0)
            if (map[(int)(newY / TILE_SIZE)][(int)(newX / TILE_SIZE)] == 0) {
                playerPos.x = newX;
                playerPos.y = newY;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);

            // Desenha teto e chão da visão 3D do lado direito para que o corredor fique visível
            DrawRectangle(600, 0, 600, 300, SKYBLUE);
            DrawRectangle(600, 300, 600, 300, DARKGRAY);

            if (wallTexture.id == 0) {
                DrawText("FALHA NO CARREGAMENTO DA TEXTURA", 610, 10, 20, RED);
            } else {
                DrawText("TEXTURA OK", 610, 10, 20, GREEN);
                DrawText(TextFormat("Caminho: %s", wallPath), 610, 35, 10, WHITE);
                DrawTextureEx(wallTexture, (Vector2){610, 55}, 0.0f, 0.25f, WHITE);
            }

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
                if (distance <= 0) distance = 1;
                float wallHeight = (TILE_SIZE * 600) / distance; 
                if (wallHeight > 600) wallHeight = 600;

                // Desenha a coluna vertical da textura da parede no lado direito da tela (offset de 600px)
                Rectangle destRect = { 600 + (i * 10), (600 - wallHeight) / 2, 9, wallHeight };
                if (wallTexture.id != 0) {
                    float texXf = fmod(x, wallTexture.width);
                    if (texXf < 0) texXf += wallTexture.width;
                    int texX = (int)texXf;
                    Rectangle sourceRect = { (float)texX, 0, 1, (float)wallTexture.height }; // Coluna correta da textura
                    Vector2 origin = { 0, 0 };
                    DrawTexturePro(wallTexture, sourceRect, destRect, origin, 0.0f, WHITE);
                } else {
                    // Fallback visual se a textura não carregou
                    DrawRectangleRec(destRect, ORANGE);
                }
            }

            DrawCircleV(playerPos, 5, RED); // Jogador
        EndDrawing();
    }

    // Descarregar a textura para liberar memória
    UnloadTexture(wallTexture);

    CloseWindow();
    return 0;
}