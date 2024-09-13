#include "raylib.h"

int main()
{
    const int screen_width = 1920;
    const int screen_height = 1080;

    InitWindow(screen_width, screen_height, "drawing exp");
    SetTargetFPS(60);
    Texture2D ship = LoadTexture("./resources/images/ship.png");
    Texture2D ufo = LoadTexture("./resources/images/ufo.png");
    Texture2D rock1 = LoadTexture("./resources/images/rock1.png");
    Texture2D rock2 = LoadTexture("./resources/images/rock2.png");
    Texture2D rock3 = LoadTexture("./resources/images/rock3.png");
    Texture2D rock4 = LoadTexture("./resources/images/rock4.png");

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        // DrawText("ASTEROIDS", screen_width/4, screen_height/2, 80, RAYWHITE);
        DrawTexture(ship, 50, 50, RAYWHITE);
        DrawTexture(ufo, 200, 50, RAYWHITE);
        DrawTexture(rock1, 350, 50, RAYWHITE);
        DrawTexture(rock2, 500, 50, RAYWHITE);
        DrawTexture(rock3, 700, 50, RAYWHITE);
        DrawTexture(rock4, 900, 50, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}