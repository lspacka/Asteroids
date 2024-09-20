//  -lraylib -lgdi32 -lwinmm -Wall -std=c99

#include "raylib.h"
// #include "physics.h"
// #include "raysynth.h"
// #include "animations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// explosion animation function:
//  - pass texture as argument
//  - unload texture
//  - do animation
//  - if its ship texture, load it again after a certain time

int RandPos(int a, int b)
{
    return a + rand() % (b - a+1);
}

// float RandSpeed(float a, float b)
// {
//     return a + ((float)rand() / (float)RAND_MAX) * (b - a);
// }

// float RandSpeed(float a, float b) {
//     float randValue = (float)rand() / (float)RAND_MAX;
//     float scaledValue = randValue * (b - a);
//     float finalValue = a + scaledValue;

//     return finalValue;
// }

typedef struct Ship {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    Rectangle bounds;
    float rotation;
    float scale;
    int lifes;
    bool active;
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
    int dir;
    bool active;
} Asteroid;

typedef struct UFO {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    Rectangle bounds;
    float scale;
    bool active;
} UFO;

int main()
{
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));

    // float rand_speed;
    // rand_speed = RandSpeed(-1.5f, 1.5f);
    // printf("rand: %f\n", rand_speed);

    int width = 800;
    int height = 800;

    InitWindow(width, height, "Asteroids");
    SetTargetFPS(60);

    const float ship_rotation = 0.5;
    const float ufo_speed = 0.9;
    const float astr_scale =  0.703125;
    const float astr_speed = 1.5;
    const int astr_num = 8;
    int i;

    Texture2D tex_ship  = LoadTexture("./resources/images/ship.png");
    Texture2D tex_ufo   = LoadTexture("./resources/images/ufo.png");
    Texture2D tex_astr1 = LoadTexture("./resources/images/rock1.png");
    Texture2D tex_astr2 = LoadTexture("./resources/images/rock2.png");
    Texture2D tex_astr3 = LoadTexture("./resources/images/rock3.png");
    Texture2D tex_astr4 = LoadTexture("./resources/images/rock4.png");
    Texture2D astr_sprites[4] = {tex_astr1, tex_astr2, tex_astr3, tex_astr4};

    // divide screen into 4 quadrants
    // Rectangle quad_1 = {0, 0, width/2, height/2};           // top left
    // Rectangle quad_2 = {width/2, 0, width, height/2};       // top right
    // Rectangle quad_3 = {0, height/2, width/2, height};      // bottom left
    // Rectangle quad_4 = {width/2, height/2, width, height};  // bottom right

    Ship ship = { 0 };
    UFO sluggo = { 0 };
    UFO mr_bill = { 0 };
    Asteroid asteroids[10] = { 0 };

    ship.pos = (Vector2){50, 50};
    ship.size = (Vector2){96, 80};
    // ship.speed = (Vector2){1.5f, 0.0f};
    ship.scale = 1.0;
    ship.active = true;
    ship.bounds = (Rectangle){ship.pos.x, ship.pos.y, ship.size.x, ship.size.y};

    sluggo.pos = (Vector2){100, 100};

    // init asteroids
    Texture2D tex_astr;
    int x_pos;
    int y_pos;
    int spr_ind;
    // float size;
    float rotation;
    int direction;
    // size = 128 * astr_scale;

    for (i = 0; i < astr_num; i++) {
        x_pos = RandPos(-10, width+10);
        y_pos = RandPos(-10, height+10);
        rotation = ((float)rand() / RAND_MAX) * 360.0f;
        direction = rand() % 4;
        spr_ind = rand() % 4;
        tex_astr = astr_sprites[spr_ind];

        asteroids[i].pos = (Vector2){x_pos, y_pos};
        asteroids[i].speed = (Vector2){1.7, 1.7};
        asteroids[i].size = (Vector2){128, 128};
        asteroids[i].bounds = (Rectangle){asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].size.x, asteroids[i].size.y};
        asteroids[i].rotation = rotation;
        asteroids[i].dir = direction;
        asteroids[i].scale = 1.0;
        asteroids[i].active = true;
        asteroids[i].tex = tex_astr;

        // printf("sprite size: %f\n", size);
        // printf("asteroid - pos.x: %f, pos.y: %f\n", asteroids[i].pos.x, asteroids[i].pos.y);
        // printf("spr_ind: %d\n", spr_ind);
    }
    
    ////////////////////////////////////////////////////////// GAME LOOP //////////////////////////////////////////////////////////

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyDown(KEY_RIGHT)) ship.pos.x += ship.speed.x;
        if (IsKeyDown(KEY_LEFT)) ship.pos.x -= ship.speed.x;

        for (i = 0; i < astr_num; i++) {
            // ship movement and collisions
            if (CheckCollisionPointRec(asteroids[i].pos, ship.bounds)) {
                asteroids[i].active = false;
                ship.active = false;
                break;
            }
        }

        // asteroids pos update and wrap-around logic
        for (i = 0; i < astr_num; i++) {
            // shitty wrap-around for full size sprites
            if (asteroids[i].pos.x-asteroids[i].size.x >= width) asteroids[i].pos.x = 0 - asteroids[i].size.x;
            else if (asteroids[i].pos.x+asteroids[i].size.x <= 0) asteroids[i].pos.x = width + asteroids[i].size.x;

            if (asteroids[i].pos.y-asteroids[i].size.y >= height) asteroids[i].pos.y = 0 - asteroids[i].size.y;
            else if (asteroids[i].pos.y+asteroids[i].size.y <= 0) asteroids[i].pos.y = height +  asteroids[i].size.y;

            // pos update
            switch(asteroids[i].dir) {
                case 0:
                    asteroids[i].pos.x += astr_speed;
                    asteroids[i].pos.y += astr_speed;
                    break;
                case 1:
                    asteroids[i].pos.x -= astr_speed;
                    asteroids[i].pos.y -= astr_speed;
                    break;
                case 2:
                    asteroids[i].pos.x += astr_speed;
                    asteroids[i].pos.y -= astr_speed;
                    break;
                case 3:
                    asteroids[i].pos.x -= astr_speed;
                    asteroids[i].pos.y += astr_speed;
            }
        }

        // draw sprites
        for (i = 0; i < astr_num; i++) {
            if (asteroids[i].active) {
                DrawTextureEx(asteroids[i].tex, asteroids[i].pos, asteroids[i].rotation, asteroids[i].scale, RAYWHITE);
            }
        }

        // draw ship and ufos
        if (ship.active) {
            DrawTextureEx(tex_ship, ship.pos, ship.rotation, ship.scale, RAYWHITE);
        }

        //  wrap around tests
        // if ((rock2.pos.x ) >= width) {
        //     rock2.pos.x = 0 - rock2.size.x;
        // }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}