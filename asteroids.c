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
    Vector2 dir;
    Vector2 center;
    Rectangle bounds;
    Rectangle rect;
    Texture2D tex;
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

    int width = 800;
    int height = 800;

    InitWindow(width, height, "Asteroids");
    SetTargetFPS(60);

    const float rotation_speed = 1.5;
    const float ufo_speed = 0.9;
    const float ast_scale =  0.703125;
    const float ast_speed = 1.5;
    const int ast_num = 8;
    bool debug = false;
    int i;

    Texture2D tex_ship  = LoadTexture("./resources/images/ship.png");
    Texture2D tex_ufo   = LoadTexture("./resources/images/ufo.png");
    Texture2D tex_ast1 = LoadTexture("./resources/images/rock1.png");
    Texture2D tex_ast2 = LoadTexture("./resources/images/rock2.png");
    Texture2D tex_ast3 = LoadTexture("./resources/images/rock3.png");
    Texture2D tex_ast4 = LoadTexture("./resources/images/rock4.png");
    Texture2D ast_sprites[4] = {tex_ast1, tex_ast2, tex_ast3, tex_ast4};

    Ship ship = { 0 };
    UFO sluggo = { 0 };
    UFO mr_bill = { 0 };
    Asteroid asteroids[10] = { 0 };

    ship.tex = tex_ship;
    ship.pos = (Vector2){width/2, height/2};
    ship.speed = (Vector2){1.5f, 0.0f};
    ship.size = (Vector2){96, 80};
    ship.center = (Vector2){ship.tex.width/2, ship.tex.height/2};
    ship.rect = (Rectangle){0, 0, ship.tex.width, ship.tex.height};
    ship.bounds = (Rectangle){ship.pos.x, ship.pos.y, ship.size.x, ship.size.y};
    // ship.rotation = 40;
    ship.scale = 0.5;
    ship.active = true;
    // ship.bounds = (Rectangle){ship.pos.x, ship.pos.y, ship.size.x, ship.size.y};
    
    Rectangle ship_rect = {0, 0, tex_ship.width, tex_ship.height};
    Vector2 ship_center = {tex_ship.width/2, tex_ship.height/2};

    sluggo.pos = (Vector2){100, 100};

    // init asteroids
    Texture2D tex_ast;
    int x_pos;
    int y_pos;
    int spr_ind;
    // float size;
    float rotation;
    int direction;
    // size = 128 * ast_scale;

    for (i = 0; i < ast_num; i++) {
        x_pos = RandPos(-10, width+10);
        y_pos = RandPos(-10, height+10);
        rotation = ((float)rand() / RAND_MAX) * 360.0f;
        direction = rand() % 4;
        spr_ind = rand() % 4;
        tex_ast = ast_sprites[spr_ind];

        asteroids[i].pos = (Vector2){x_pos, y_pos};
        asteroids[i].speed = (Vector2){1.7, 1.7};
        asteroids[i].size = (Vector2){128, 128};
        asteroids[i].bounds = (Rectangle){asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].size.x, asteroids[i].size.y};
        asteroids[i].rotation = rotation;
        asteroids[i].dir = direction;
        asteroids[i].scale = 1.0;
        asteroids[i].active = true;
        asteroids[i].tex = tex_ast;
    }

    ////////////////////////////////////////////////////////// GAME LOOP //////////////////////////////////////////////////////////

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_D)) debug = !debug;

        if (IsKeyDown(KEY_RIGHT)) ship.rotation += rotation_speed;
        if (IsKeyDown(KEY_LEFT)) ship.rotation -= rotation_speed;

        // for (i = 0; i < ast_num; i++) {
        //     // ship movement and collisions
        //     if (CheckCollisionPointRec(asteroids[i].pos, ship.bounds)) {
        //         asteroids[i].active = false;
        //         ship.active = false;
        //         break;
        //     }
        // }

        // asteroids pos update and wrap-around logic
        for (i = 0; i < ast_num; i++) {
            // shitty wrap-around for full size sprites
            if (asteroids[i].pos.x-asteroids[i].size.x >= width) asteroids[i].pos.x = 0 - asteroids[i].size.x;
            else if (asteroids[i].pos.x+asteroids[i].size.x <= 0) asteroids[i].pos.x = width + asteroids[i].size.x;

            if (asteroids[i].pos.y-asteroids[i].size.y >= height) asteroids[i].pos.y = 0 - asteroids[i].size.y;
            else if (asteroids[i].pos.y+asteroids[i].size.y <= 0) asteroids[i].pos.y = height +  asteroids[i].size.y;

            // pos update
            switch(asteroids[i].dir) {
                case 0:
                    asteroids[i].pos.x += ast_speed;
                    asteroids[i].pos.y += ast_speed;
                    break;
                case 1:
                    asteroids[i].pos.x -= ast_speed;
                    asteroids[i].pos.y -= ast_speed;
                    break;
                case 2:
                    asteroids[i].pos.x += ast_speed;
                    asteroids[i].pos.y -= ast_speed;
                    break;
                case 3:
                    asteroids[i].pos.x -= ast_speed;
                    asteroids[i].pos.y += ast_speed;
                    break;
            }
        }

        // draw asteroids
        for (i = 0; i < ast_num; i++) {
            if (asteroids[i].active) {
                DrawTextureEx(asteroids[i].tex, asteroids[i].pos, asteroids[i].rotation, asteroids[i].scale, RAYWHITE);
            }
        }

        // draw ship and ufos
        if (ship.active) {
            DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);
        }

        // debug section
        if (debug) {
            // DrawText("Ship Data:", 20, height-80, 20, WHITE);
            DrawText(TextFormat("Rotation: %.2f", ship.rotation), 20, height-70, 20, WHITE);
            DrawText(TextFormat("Angle: %.2f", ship.dir), 20, height-50, 20, WHITE);
            DrawText(TextFormat("Direction: %.2f", ship.dir), 20, height-30, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}