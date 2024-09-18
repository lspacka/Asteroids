//  -lraylib -lgdi32 -lwinmm -Wall -std=c99

#include "raylib.h"
// #include "physics.h"
// #include "raysynth.h"
// #include "animations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int RandPos(int a, int b)
{
    return a + rand() % (b - a+1);
}


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
    Texture2D tex;
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
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));

    int width = 800;
    int height = 800;

    InitWindow(width, height, "Asteroids");
    SetTargetFPS(60);

    int rand_pos = RandPos(-90, width+10);
    // printf("rand_pos: %d\n", rand_pos);
    
    const float ship_rotation = 0.5;
    const float ufo_speed = 0.9;
    const float astr_scale =  0.703125;
    int i;

    Texture2D tex_ship  = LoadTexture("./resources/images/ship.png");
    Texture2D tex_ufo   = LoadTexture("./resources/images/ufo.png");
    Texture2D tex_astr1 = LoadTexture("./resources/images/rock1.png");
    Texture2D tex_astr2 = LoadTexture("./resources/images/rock2.png");
    Texture2D tex_astr3 = LoadTexture("./resources/images/rock3.png");
    Texture2D tex_astr4 = LoadTexture("./resources/images/rock4.png");
    Texture2D astr_sprites[4] = {tex_astr1, tex_astr2, tex_astr3, tex_astr4};

    // divide screen into 4 quadrants

    Ship ship = { 0 };
    UFO sluggo = { 0 };
    UFO mr_bill = { 0 };
    Asteroid rock1 = { 0 };
    Asteroid rock2 = { 0 };
    // Asteroid rock3 = { 0 };
    // Asteroid rock4 = { 0 };
    Asteroid asteroids[10] = { 0 };

    ship.pos = (Vector2){50, 50};
    ship.speed = (Vector2){1.5f, 0.0f};

    sluggo.pos = (Vector2){100, 100};

    // asteroids creation - init vals and sprite drawing
    Texture2D tex_astr;
    int x_pos;
    int y_pos;
    int spr_ind;
    float rotation;    

    for (i = 0; i < 10; i++) {
        x_pos = RandPos(-10, width+10);
        y_pos = RandPos(-10, height+10);
        rotation = ((float)rand() / RAND_MAX) * 360.0f;
        spr_ind = rand() % 4;
        tex_astr = astr_sprites[spr_ind];

        asteroids[i].pos = (Vector2){x_pos, y_pos};
        asteroids[i].speed = (Vector2){1.7, 1.7};
        asteroids[i].size = (Vector2){128, 128};
        asteroids[i].bounds = (Rectangle){asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].size.x, asteroids[i].size.y};
        asteroids[i].rotation = rotation;
        asteroids[i].tex = tex_astr;

        // printf("asteroid - pos.x: %f, pos.y: %f\n", asteroids[i].pos.x, asteroids[i].pos.y);
        // printf("spr_ind: %d\n", spr_ind);
    }
    printf("///////////////////////////////////////////////\n");

    // asteroids pos update and wrap-around logic
    // try with random values first

    rock1.pos = (Vector2){100, 100};
    rock1.speed = (Vector2){1.5, 1.5};
    rock1.size = (Vector2){90, 90};
    rock1.bounds = (Rectangle){rock1.pos.x, rock1.pos.y, rock1.size.x, rock1.size.y};

    rock2.pos = (Vector2){100, 100};
    rock2.speed = (Vector2){1.5};
    rock2.size = (Vector2){90, 90};
    rock2.bounds = (Rectangle){rock2.pos.x, rock2.pos.y, rock2.size.x, rock2.size.y};

    // rock3.pos = (Vector2){100, 100};
    // rock3.speed = (Vector2){1.5, 1.5};
    bool loop = false;

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyDown(KEY_RIGHT)) ship.pos.x += ship.speed.x;
        if (IsKeyDown(KEY_LEFT)) ship.pos.x -= ship.speed.x;

        // rock2.pos.y += rock2.speed.y;
        // rock3.pos.x += rock1.speed.x;
        // rock3.pos.y += rock1.speed.y;

        // draw sprites
        for (i = 0; i < 10; i++) {
            DrawTextureEx(asteroids[i].tex, asteroids[i].pos, asteroids[i].rotation, astr_scale, RAYWHITE);
        }

        //  wrap around tests
        // if ((rock2.pos.x ) >= width) {
        //     rock2.pos.x = 0 - rock2.size.x;
        // }

        // DrawTextureEx(tex_astr1, rock1.pos, 0.0, astr_scale, RAYWHITE);
        // DrawTextureEx(tex_astr2, rock2.pos, 0.0, astr_scale, RAYWHITE);

        EndDrawing();
    }

    // printf("\nscreen width: %d\n", screen_width);
    // printf("screen height: %d\n\n", screen_height);

    CloseWindow();

    return 0;
}