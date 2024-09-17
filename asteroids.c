// -lraylib -lgdi32 -lwinmm -Wall -std=c99

#include "raylib.h"
// #include "physics.h"
// #include "raysynth.h"
// #include "animations.h"
#include <stdio.h>

typedef struct Ship {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    Rectangle bounds;
    float rotation;
    int lifes;
} Ship;

typedef struct Asteroid {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    Rectangle bounds;
    float rotation;
    float scale;
} Asteroid;

typedef struct UFO {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    float scale;
    Rectangle bounds;
} UFO;

int main()
{
    int width = 600;
    int height = 600;
    
    const float ship_rotation = 0.5;
    const float ufo_speed = 0.9;
    const float ast_scale =  0.703125;

    InitWindow(width, height, "Asteroids");
    SetTargetFPS(60);

    Ship ship = { 0 };
    ship.pos = (Vector2){50, 50};
    ship.speed = (Vector2){1.5f, 0.0f};

    UFO sluggo = { 0 };
    sluggo.pos = (Vector2){100, 100};

    UFO mr_bill = { 0 };

    Asteroid rock1 = { 0 };
    rock1.pos = (Vector2){100, 100};
    rock1.speed = (Vector2){1.5, 1.5};
    rock1.size = (Vector2){90, 90};
    rock1.bounds = (Rectangle){rock1.pos.x, rock1.pos.y, rock1.size.x, rock1.size.y};

    Asteroid rock2 = { 0 };
    rock2.pos = (Vector2){100, 100};
    rock2.speed = (Vector2){1.5};
    rock2.size = (Vector2){90, 90};
    rock2.bounds = (Rectangle){rock2.pos.x, rock2.pos.y, rock2.size.x, rock2.size.y};

    Asteroid rock3 = { 0 };
    rock3.pos = (Vector2){100, 100};
    rock3.speed = (Vector2){1.5, 1.5};

    Asteroid rock4 = { 0 };

    Texture2D tex_ship  = LoadTexture("./resources/images/ship.png");
    Texture2D tex_ufo   = LoadTexture("./resources/images/ufo.png");
    Texture2D tex_rock1 = LoadTexture("./resources/images/rock1.png");
    Texture2D tex_rock2 = LoadTexture("./resources/images/rock2.png");
    Texture2D tex_rock3 = LoadTexture("./resources/images/rock3.png");
    Texture2D tex_rock4 = LoadTexture("./resources/images/rock4.png");


    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyDown(KEY_RIGHT)) ship.pos.x += ship.speed.x;
        if (IsKeyDown(KEY_LEFT)) ship.pos.x -= ship.speed.x;

        rock1.pos.x += rock1.speed.x;
        rock1.pos.y += rock1.speed.y;

        rock2.pos.x += rock2.speed.x;
        // rock2.pos.y += rock2.speed.y;

        // rock3.pos.x += rock1.speed.x;
        // rock3.pos.y += rock1.speed.y;

        //  wrap around test
        if ((rock2.pos.x ) >= width) {
            rock2.pos.x = 0 - rock2.size.x;
        }

        // DrawTextureEx(tex_ship, ship.pos, 0.0, 0.7, RAYWHITE);
        DrawTextureEx(tex_rock1, rock1.pos, 0.0, ast_scale, RAYWHITE);
        DrawTextureEx(tex_rock2, rock2.pos, 0.0, ast_scale, RAYWHITE);

        EndDrawing();
    }

    // printf("\nscreen width: %d\n", screen_width);
    // printf("screen height: %d\n\n", screen_height);

    CloseWindow();

    return 0;
}