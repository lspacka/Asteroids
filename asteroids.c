//  -lraylib -lgdi32 -lwinmm -Wall -std=c99

#include "raylib.h"
// #include "physics.h"
// #include "raysynth.h"
// #include "animations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct Ship {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    Vector2 dir;
    Vector2 center;
    Vector2 tip;
    Rectangle bounds;
    Rectangle rect;
    Texture2D tex;
    float rotation;
    // float tipX;
    // float tipY;
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

typedef struct Torp {
    Vector2 pos;
    Vector2 speed;
    Vector2 dir;
    Texture2D tex;
    Rectangle bounds;
    bool active;
} Torp;

int RandPos(int a, int b)
{
    return a + rand() % (b - a+1);
}

void AstBlast(Asteroid ast)
{
    // 
}

// Torp Fire(Ship ship, Texture2D tex)
// {
//     Torp torp = { 0 };
//     torp.active = true;
//     torp.pos = (Vector2){ship.tip.x, ship.tip.y};
//     torp.speed = (Vector2){1.0, 1.0};
//     torp.dir = (Vector2){ship.dir.x, ship.dir.y};
//     torp.tex = tex;

//     return torp;
// }

int main()
{
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));

    int screen_width = 900;
    int screen_height = 900;

    InitWindow(screen_width, screen_height, "Asteroids");
    SetTargetFPS(60);

    const float rotation_speed = 1.5;
    const float ufo_speed = 0.9;
    const float ast_scale =  0.703125;
    const float ast_speed = 1.5;
    const int ast_num = 8;
    bool debug = false;
    int i;

    Texture2D tex_ship  = LoadTexture("./resources/images/ship.png");
    Texture2D tex_torp = LoadTexture("./resources/images/torp.png");
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
    Torp torps[4] = { 0 };

    ship.tex = tex_ship;
    ship.pos = (Vector2){screen_width/2, screen_height/2};
    ship.speed = (Vector2){1.5f, 0.0f};
    ship.size = (Vector2){ship.tex.width, ship.tex.height};
    ship.center = (Vector2){ship.tex.width/2, ship.tex.height/2};
    ship.rect = (Rectangle){0, 0, ship.tex.width, ship.tex.height};
    ship.bounds = (Rectangle){ship.pos.x, ship.pos.y, ship.size.x, ship.size.y};
    ship.rotation = -90;
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
        x_pos = RandPos(-10, screen_width+10);
        y_pos = RandPos(-10, screen_height+10);
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

    // init torps
    int torp_index = 0;
    for (i = 0; i < 4; i++) {
        torps[i].active = false;
        torps[i].speed = (Vector2){3.0, 3.0};
        torps[i].tex = tex_torp;
    }

    ////////////////////////////////////////////////////////// GAME LOOP //////////////////////////////////////////////////////////

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_D)) debug = !debug;

        if (IsKeyDown(KEY_RIGHT)) {
            ship.rotation += rotation_speed;
            // for (i = 0; i < 4; i++) {
            //     torps[i].pos = (Vector2){ship.tip.x, ship.tip.y};
            //     torps[i].dir = (Vector2){ship.dir.x, ship.dir.y};
            // }
        }
        if (IsKeyDown(KEY_LEFT)) {
            ship.rotation -= rotation_speed;
            // for (i = 0; i < 4; i++) {
            //     torps[i].pos = (Vector2){ship.tip.x, ship.tip.y};
            //     torps[i].dir = (Vector2){ship.dir.x, ship.dir.y};
            // }
        }
        // if (IsKeyDown(KEY_UP)) ship.thrust;
        // if (IsKeyDown(KEY_LEFT_CONTROL)) ship.fire;
        // if (IsKeyDown(KEY_SPACE)) ship.hyperspace;

        // calc ship's direction
        float angle = ship.rotation * (PI/180.0);
        float dir_x = cos(angle - PI/2);
        float dir_y = sin(angle - PI/2);

        ship.dir.x = dir_x;
        ship.dir.y = dir_y;

        // shooting calc
        Vector2 dir = ship.dir;
        float cannon_offset_x = 0;
        float cannon_offset_y = -ship.tex.height/2;

        // rotate the offset based on the ship's rotation angle
        float rota_offset_x = cannon_offset_x * cos(angle) - cannon_offset_y * sin(angle);
        float rota_offset_y = cannon_offset_x * sin(angle) + cannon_offset_y * cos(angle);

        // calculate the absolute coords of the cannon tip
        ship.tip.x = ship.pos.x + rota_offset_x;
        ship.tip.y = ship.pos.y + rota_offset_y;

        // // update tip coords in torps before shooting
        // for (i = 0; i < 4; i++) {
        //     torps[i].pos = (Vector2){ship.tip.x, ship.tip.y};
        //     torps[i].dir = (Vector2){ship.dir.x, ship.dir.y};
        // }
        
        if (IsKeyPressed(KEY_LEFT_CONTROL)) {
            // DrawCircle(ship.tip.x, ship.tip.y, 5.0, RED);
            // Torp torp = Fire(ship, tex_torp);
            // torp.active = true;
            // torp.pos = (Vector2){ship.tip.x, ship.tip.y};
            // torp.speed = (Vector2){1.0, 1.0};
            // torp.dir = (Vector2){ship.dir.x, ship.dir.y};
            // torp.tex = tex_torp;
            // update tip coords in torps before shooting
            for (i = 0; i < 4; i++) {
                torps[i].pos = (Vector2){ship.tip.x, ship.tip.y};
                torps[i].dir = (Vector2){ship.dir.x, ship.dir.y};
            }
            
            if (torp_index == 3) {
                torp_index = 0;
                // after some time:
                // for (i = 0; i < 4; i++) {
                //     torps[i].active = false;
                // }
            }
            torps[torp_index].active = true;
            // torps[torp_index].pos.x += torps[torp_index].dir.x * torps[torp_index].speed.x;
            // torps[torp_index].pos.y += torps[torp_index].dir.y * torps[torp_index].speed.y;
            // DrawTextureEx(tex_torp, torps[torp_index].pos, 0.0, 1.0, RAYWHITE);

            // torp_index++;
        }

        if (torps[torp_index].active) {
                torps[torp_index].pos.x += torps[torp_index].dir.x * torps[torp_index].speed.x;
                torps[torp_index].pos.y += torps[torp_index].dir.y * torps[torp_index].speed.y;
                DrawTextureEx(tex_torp, torps[torp_index].pos, 0.0, 1.0, RAYWHITE);
        }

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
            if (asteroids[i].pos.x-asteroids[i].size.x >= screen_width) asteroids[i].pos.x = 0 - asteroids[i].size.x;
            else if (asteroids[i].pos.x+asteroids[i].size.x <= 0) asteroids[i].pos.x = screen_width + asteroids[i].size.x;

            if (asteroids[i].pos.y-asteroids[i].size.y >= screen_height) asteroids[i].pos.y = 0 - asteroids[i].size.y;
            else if (asteroids[i].pos.y+asteroids[i].size.y <= 0) asteroids[i].pos.y = screen_height +  asteroids[i].size.y;

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
            DrawText(TextFormat("Cannon Tip - X: %.2f, Y: %.2f", ship.tip.x, ship.tip.y), 20, screen_height-90, 20, WHITE);
            DrawText(TextFormat("Rotation: %.2f", ship.rotation), 20, screen_height-70, 20, WHITE);
            DrawText(TextFormat("Direction: %.2f", ship.dir), 20, screen_height-50, 20, WHITE);
            DrawText(TextFormat("Torp Index: %d", torp_index), 20, screen_height-30, 20, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}