//  -lraylib -lgdi32 -lwinmm -Wall -std=c99 -I c:/raylib/raylib/src

#include "raylib.h"
#include "raymath.h"
// #include "physics.h"
// #include "raysynth.h"
// #include "animations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// #define PI 3.14159265358979323846

typedef struct Ship {
    Vector2 pos;
    Vector2 vel;
    Vector2 accel;      // 
    Vector2 size;
    Vector2 dir;
    Vector2 center;
    Vector2 tip;
    Rectangle bounds;
    Rectangle rect;
    Texture2D tex;
    float rotation;
    float lowSpeed;
    float scale;
    int lifes;
    bool active;
} Ship;

typedef struct Asteroid {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    Vector2 center;
    Rectangle rect;
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
    Vector2 size;
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
    bool debug = false;
    int ast_num = 8;
    int i, j;

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
    ship.size = (Vector2){ship.tex.width, ship.tex.height};
    ship.center = (Vector2){ship.tex.width/2, ship.tex.height/2};
    ship.rect = (Rectangle){0, 0, ship.tex.width, ship.tex.height};
    ship.bounds = (Rectangle){ship.pos.x, ship.pos.y, ship.size.x, ship.size.y};
    // ship.bounds = (Rectangle){ship.pos.x-ship.center.x, ship.pos.y-ship.center.y, ship.size.x, ship.size.y}; // offset bbox
    // ship.rotation = -90;
    ship.scale = 0.5;
    ship.lowSpeed = 0.004;
    ship.active = true;

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

        asteroids[i].tex = tex_ast;
        asteroids[i].pos = (Vector2){x_pos, y_pos};
        asteroids[i].speed = (Vector2){1.7, 1.7};
        asteroids[i].size = (Vector2){128, 128};
        asteroids[i].center = (Vector2){asteroids[i].tex.width/2, asteroids[i].tex.height/2};
        asteroids[i].rect = (Rectangle){0, 0, asteroids[i].tex.width, asteroids[i].tex.height};
        asteroids[i].rotation = rotation;
        asteroids[i].bounds = (Rectangle){asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].tex.width, asteroids[i].tex.height};
        // asteroids[i].bounds = (Rectangle){asteroids[i].center.x, asteroids[i].center.y, asteroids[i].tex.width, asteroids[i].tex.height};
        asteroids[i].dir = direction;
        asteroids[i].scale = 1.0;
        asteroids[i].active = true;
    }   

    // init torps
    int torp_index = 0;
    for (i = 0; i < 4; i++) {
        torps[i].tex = tex_torp;
        torps[i].active = false;
        torps[i].speed = (Vector2){3.0, 3.0};
        torps[i].size = (Vector2){4, 4};
        torps[i].bounds = (Rectangle){torps[i].pos.x, torps[i].pos.y, torps[i].tex.width, torps[i].tex.height};
        // torps[i].bounds = (Rectangle){torps[i].pos.x, torps[i].pos.y, torps[i].size.x, torps[i].size.y};
    }

    // vars 4 trig calcs
    Vector2 thrust;
    Vector2 dir;
    float angle;
    float dir_x;
    float dir_y;
    float cannon_offset_x;
    float cannon_offset_y;
    float rota_offset_x;
    float rota_offset_y;
    float top_speed = 50.0f;
    float thrust_force = 0.1f;
    float friction = 0.99f;
    float drag = 0.01f; 
    float speed;

    ////////////////////////////////////////////////////////// GAME LOOP //////////////////////////////////////////////////////////

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (IsKeyPressed(KEY_D)) 
            debug = !debug;

        if (IsKeyDown(KEY_RIGHT)) 
            ship.rotation += rotation_speed;
        if (IsKeyDown(KEY_LEFT))  
            ship.rotation -= rotation_speed;

        // calc ship's direction
        angle = ship.rotation * (PI/180.0);
        dir_x = cos(angle - PI/2);
        dir_y = sin(angle - PI/2);

        ship.dir.x = dir_x;
        ship.dir.y = dir_y;

        // shooting calc
        dir = ship.dir;
        cannon_offset_x = 0;
        cannon_offset_y = -ship.tex.height/2;

        // rotate the offset based on the ship's rotation angle
        rota_offset_x = cannon_offset_x * cos(angle) - cannon_offset_y * sin(angle);
        rota_offset_y = cannon_offset_x * sin(angle) + cannon_offset_y * cos(angle);

        // calculate the absolute coords of the cannon tip
        ship.tip.x = ship.pos.x + rota_offset_x;
        ship.tip.y = ship.pos.y + rota_offset_y;

        /////////////// SHOOTING ///////////////

        if (IsKeyPressed(KEY_LEFT_CONTROL)) {
            if (torp_index == 4) {
                torp_index = 0;
            }

            // update tip coords in torps before shooting
            torps[torp_index].pos = (Vector2){ship.tip.x-ship.size.x/2, ship.tip.y-ship.size.y/2};
            torps[torp_index].dir = (Vector2){ship.dir.x, ship.dir.y};
            torps[torp_index].active = true;

            torp_index++;
        }

        // draw torps
        for (i = 0; i < 4; i++) {
            if (torps[i].active) {
                torps[i].pos.x += torps[i].dir.x * torps[i].speed.x;
                torps[i].pos.y += torps[i].dir.y * torps[i].speed.y;
                DrawTextureEx(tex_torp, torps[i].pos, 0.0, 1.0, RAYWHITE);
            }
            // wrap-around
        }

        /////////////// THRUST ///////////////
        if (IsKeyDown(KEY_UP)) {

            // Recalculate thrust direction based on the ship's current rotation
            thrust = ship.dir;
            Vector2Normalize(thrust);  // Normalize to get direction only

            // Apply thrust to velocity (incremental acceleration)
            ship.vel.x += thrust.x * thrust_force;
            ship.vel.y += thrust.y * thrust_force;

            // Optional: Cap the velocity to top speed
            speed = Vector2Length(ship.vel);
            if (speed > top_speed) {
                Vector2Normalize(ship.vel);
                ship.vel.x *= ship.lowSpeed;
                ship.vel.y *= ship.lowSpeed;
            }
        } else {
            // No thrust, but apply slight friction to slow down gradually
            ship.vel.x *= friction;
            ship.vel.y *= friction;
        }

        // Update the ship's position based on velocity
        ship.pos.x += ship.vel.x;
        ship.pos.y += ship.vel.y;

        // update ship's bounds to match the new position
        ship.bounds.x = ship.pos.x - ship.center.x;
        ship.bounds.y = ship.pos.y - ship.center.y;

        if (ship.active) {
            DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);
        }
        
        // ship wrap-around test
        if (ship.pos.x-ship.size.x >= screen_width) 
            ship.pos.x = 0 - ship.size.x;
        else if (ship.pos.x+ship.size.x <= 0)
            ship.pos.x = screen_width + ship.size.x;

        if (ship.pos.y-ship.size.y >= screen_height)
            ship.pos.y = 0 - ship.size.y;
        else if (ship.pos.y+ship.size.y <= 0)
            ship.pos.y = screen_height + ship.size.y;

        // asteroids pos update and wrap-around logic
        for (i = 0; i < ast_num; i++) {
            // shitty wrap-around for full size sprites
            if (asteroids[i].pos.x-asteroids[i].size.x >= screen_width) 
                asteroids[i].pos.x = 0 - asteroids[i].size.x;
            else if (asteroids[i].pos.x+asteroids[i].size.x <= 0) 
                asteroids[i].pos.x = screen_width + asteroids[i].size.x;

            if (asteroids[i].pos.y-asteroids[i].size.y >= screen_height) 
                asteroids[i].pos.y = 0 - asteroids[i].size.y;
            else if (asteroids[i].pos.y+asteroids[i].size.y <= 0) 
                asteroids[i].pos.y = screen_height +  asteroids[i].size.y;

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

            asteroids[i].bounds.x = asteroids[i].pos.x;
            asteroids[i].bounds.y = asteroids[i].pos.y;
        }

        // basic wrap-around for ship

        // draw asteroids
        for (i = 0; i < ast_num; i++) {
            if (asteroids[i].active) {
                DrawTextureEx(asteroids[i].tex, asteroids[i].pos, asteroids[i].rotation, asteroids[i].scale, RAYWHITE);
                // DrawTexturePro(asteroids[i].tex, asteroids[i].rect, asteroids[i].bounds, asteroids[i].center, asteroids[i].rotation, RAYWHITE);
            }
        }

        // draw ship and ufos
        if (ship.active) {
            DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);
            // DrawTextureEx(ship.tex, ship.pos, 0.0, 1.0, WHITE);
        }

        // asteroids collisions
        for (i = 0; i < ast_num; i++) {
            // if (asteroids[i].active && ship.active) {
            //     if (CheckCollisionRecs(asteroids[i].bounds, ship.bounds)) {
            //         asteroids[i].active = false;
            //         ship.active = false;
            //         // break;
            //     }
            // }

            for (j = 0; j < 4; j++) {
                if (asteroids[i].active && torps[j].active) {
                    if (CheckCollisionPointRec(torps[j].pos, asteroids[i].bounds)) {
                        asteroids[i].active = false;
                        torps[j].active = false;
                        // break;
                    }
                }
            }
        }

        /////////////// DEBUGGING DISPLAY ///////////////
        if (debug) {
            // DrawText(TextFormat("Cannon Tip - X: %.2f, Y: %.2f", ship.tip.x, ship.tip.y), 20, screen_height-90, 20, WHITE);
            DrawText(TextFormat("Rotation: %.2f", ship.rotation), 20, screen_height-90, 20, WHITE);
            DrawText(TextFormat("PosX: %.2f, ", ship.pos.x), 20, screen_height-70, 20, WHITE);
            DrawText(TextFormat("PosY: %.2f", ship.pos.y), 180, screen_height-70, 20, WHITE);
            DrawText(TextFormat("Thrust: %.2f", thrust), 20, screen_height-50, 20, WHITE);   // test 
            DrawText(TextFormat("Vel: %.2f, ", ship.vel.x), 20, screen_height-30, 20, WHITE);  // test
            // DrawText(TextFormat("Accel X: %.2f", ship.accel.x), 160, screen_height-30, 20, WHITE);         // test

            // show bounding boxes
            DrawRectangleLinesEx(ship.bounds, 1.0, GREEN);
            
            for (i = 0; i < ast_num; i++) {
                DrawRectangleLinesEx(asteroids[i].bounds, 1.0, ORANGE);
                DrawCircle(asteroids[i].pos.x, asteroids[i].pos.y, 3.0, GREEN);
            }
            
            for (i = 0; i < 4; i++) {
                DrawRectangleLinesEx(torps[i].bounds, 1.0, RED);
            }

            // signal init pos
            DrawCircle(ship.pos.x, ship.pos.y, 3.0, ORANGE);  
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}