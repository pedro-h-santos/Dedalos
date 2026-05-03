#include "raylib.h"
#include <math.h>

#define MAP_SIZE  8
#define TILE_SIZE 64
#define FOV       1.0472f   // 60 graus em radianos
#define NUM_RAYS  60
#define MARGIN    4         // margem de colisão em pixels

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

/* FIX #5 — verifica bounds antes de acessar o mapa */
int mapIsWall(float x, float y) {
    int mx = (int)(x / TILE_SIZE);
    int my = (int)(y / TILE_SIZE);
    if (mx < 0 || mx >= MAP_SIZE || my < 0 || my >= MAP_SIZE) return 1;
    return map[my][mx] == 1;
}

int main() {
    InitWindow(1200, 600, "Raycasting");

    Vector2 playerPos   = { 96, 96 };
    float   playerAngle = 0.0f;

    // --- carregamento de textura (sem alteração) ---
    Texture2D wallTexture = { 0 };
    const char *paths[] = {
        "C:/projetos/dedalos/img/parede.jpg",
        "img/parede.jpg", "..\\img\\parede.jpg"
    };
    for (int i = 0; i < 3; i++) {
        wallTexture = LoadTexture(paths[i]);
        if (wallTexture.id != 0) break;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        // --- movimento ---
        if (IsKeyDown(KEY_LEFT))  playerAngle -= 0.05f;
        if (IsKeyDown(KEY_RIGHT)) playerAngle += 0.05f;

        if (IsKeyDown(KEY_UP)) {
            float nx = playerPos.x + cos(playerAngle) * 3;
            float ny = playerPos.y + sin(playerAngle) * 3;
            // FIX #4 — colisão com margem nos 4 cantos
            if (!mapIsWall(nx+MARGIN, ny+MARGIN) && !mapIsWall(nx-MARGIN, ny-MARGIN) &&
                !mapIsWall(nx+MARGIN, ny-MARGIN) && !mapIsWall(nx-MARGIN, ny+MARGIN)) {
                playerPos.x = nx; playerPos.y = ny;
            }

        }
        if (IsKeyDown(KEY_DOWN)) {
            float nx = playerPos.x - cos(playerAngle) * 3;
            float ny = playerPos.y - sin(playerAngle) * 3;
            if (!mapIsWall(nx+MARGIN, ny+MARGIN) && !mapIsWall(nx-MARGIN, ny-MARGIN) &&
                !mapIsWall(nx+MARGIN, ny-MARGIN) && !mapIsWall(nx-MARGIN, ny+MARGIN)) {
                playerPos.x = nx; playerPos.y = ny;
            }

        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(600, 0,   600, 300, SKYBLUE);
        DrawRectangle(600, 300, 600, 300, DARKGRAY);

        // --- mapa 2D ---
        for (int y = 0; y < MAP_SIZE; y++)
            for (int x = 0; x < MAP_SIZE; x++)
                if (map[y][x] == 1)
                    DrawRectangle(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE-1, TILE_SIZE-1, DARKGRAY);

        // --- raycasting ---
        for (int i = 0; i < NUM_RAYS; i++) {

            // FIX #1 — FOV de 60° em radianos, distribuído entre NUM_RAYS raios
            float rayAngle = (playerAngle - FOV/2.0f) +
                              (((float)i / (NUM_RAYS - 1)) * FOV);


            float distance = 0;
            float rx = playerPos.x, ry = playerPos.y;

            // FIX #5 — usa mapIsWall() com checagem de bounds
            while (!mapIsWall(rx, ry) && distance < 600) {

                distance += 1;
                rx = playerPos.x + cos(rayAngle) * distance;
                ry = playerPos.y + sin(rayAngle) * distance;
            }

            DrawLineV(playerPos, (Vector2){rx, ry}, YELLOW);

            // FIX #3 — correção fish-eye: multiplica pelo cosseno do ângulo relativo
            float correctedDist = distance * cos(rayAngle - playerAngle);

            if (correctedDist < 1) correctedDist = 1;
            float wallHeight = (TILE_SIZE * 600) / correctedDist;
            if (wallHeight > 600) wallHeight = 600;

            Rectangle destRect = { 600 + (i * 10), (600 - wallHeight) / 2, 9, wallHeight };

            if (wallTexture.id != 0) {
                // FIX #2 — texX usa a coordenada local no tile (0..TILE_SIZE)
                //          detecta parede vertical vs horizontal pelo resto dominante
                float hitX = fmod(rx, TILE_SIZE);
                float hitY = fmod(ry, TILE_SIZE);
                float texXf;
                // se o raio bateu mais perto de borda vertical, usa hitY; senão hitX
                float dX = fmin(hitX, TILE_SIZE - hitX);
                float dY = fmin(hitY, TILE_SIZE - hitY);
                texXf = (dX < dY)
                    ? (hitY / TILE_SIZE) * wallTexture.width
                    : (hitX / TILE_SIZE) * wallTexture.width;
                if (texXf < 0) texXf += wallTexture.width;

                Rectangle srcRect = { texXf, 0, 1, (float)wallTexture.height };
                DrawTexturePro(wallTexture, srcRect, destRect, (Vector2){0,0}, 0.0f, WHITE);
            } else {
                DrawRectangleRec(destRect, ORANGE);
            }
        }

        DrawCircleV(playerPos, 5, RED);
        EndDrawing();
    }

    UnloadTexture(wallTexture);
    CloseWindow();
    return 0;
}