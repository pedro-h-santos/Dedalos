#include "raylib.h"
#include "../player/player.h"
#include "../map/map.h"
#include "renderer/renderer.h"

int main(void) {
    InitWindow(800, 450, "Masmorrearemos");
    SetTargetFPS(60);

    struct Map m;
    Map_init(&m);         // troque por Map_generate(&map, 42) para mapa aleatório

    struct Player p;
    Player_init(&p, 5.0f, 5.0f);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Player_handleInput(&p, &m, dt);

        BeginDrawing();
            ClearBackground(BLACK);
            Renderer_drawScene(&p, &m);
            DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}