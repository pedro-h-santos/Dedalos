#include "raylib.h"
#include "player/player.h"
#include "map/map.h"
#include "renderer/renderer.h"

int main(void) {
    InitWindow(800, 450, "Masmorrearemos");
    SetTargetFPS(60);

    Texture2D textura_parede = LoadTexture("img/parede.bmp");

    Map m;
    Map_init(&m);         // troque por Map_generate(&map, 42) para mapa aleatório

    Player p;
    Player_init(&p, 4.0f, 5.0f);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Player_handle_input(&p, &m, dt);

        BeginDrawing();
            ClearBackground(BLACK);
            Renderer_scene(&m, &p, &textura_parede);
            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadTexture(textura_parede);
    CloseWindow();
    return 0;
}
