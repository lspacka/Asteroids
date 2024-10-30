//  -lraylib -lgdi32 -lwinmm -Wall -std=c99 -I c:/raylib/raylib/src
//  asteroids: #636363
//  ufo: #c6c6c6
//  ship: #b9b9b9

#include "raylib.h"
#include "raymath.h"
// #include "assynth.h"
// #include "animations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


typedef struct Ship {
    Vector2 pos;
    Vector2 vel;
    Vector2 accel;      // 
    Vector2 size;
    Vector2 dir;
    Vector2 center;
    Vector2 circle_center;
    Vector2 tip;
    Rectangle bounds;
    Rectangle rect;
    Texture2D tex;
    Color* pix;
    float rotation;     //
    float lowSpeed;
    float scale;
    float radius;
    int lifes;
    bool active;
} Ship;

typedef struct Asteroid {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    Vector2 center;
    Vector2 circle_center;
    Rectangle rect;
    Rectangle bounds;
    Texture2D tex;
    Color* pix;
    float rotation;
    float scale;
    // float circle_center;
    float radius;
    int dir;
    bool active;
} Asteroid;

typedef struct UFO {
    Vector2 pos;
    Vector2 speed;
    Vector2 accel;
    Vector2 size;
    Rectangle bounds;
    Color* pix;
    float scale;
    bool active;
} UFO;

typedef struct Torp {
    Vector2 pos;
    Vector2 speed;
    Vector2 dir;
    Vector2 size;
    Vector2 center;
    Rectangle source;
    Rectangle dest;
    Texture2D tex;
    Rectangle bounds;
    Color* pix;
    float radius;
    bool active;
} Torp;

int RandPos(int a, int b);
void AstBlast(Asteroid ast);

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
    const float scale = 0.7f;
    int ast_num = 10;
    int i, j;
    bool debug = false;
    bool collision_found = false;
    Vector2 mouse_pos;

    Ship ship = { 0 };
    UFO sluggo = { 0 };
    UFO mr_bill = { 0 };
    Asteroid asteroids[10] = { 0 };
    Torp torps[4] = { 0 };

    /////////////// IMAGES AND TEXTURES SETUP ///////////////

    Image ship_img = LoadImage("./resources/images/ship.png");
    Image torp     = LoadImage("./resources/images/torp.png");
    Image ufo      = LoadImage("./resources/images/ufo.png");

    Image ast1   = LoadImage("./resources/images/ast1.png");
    Image ast1_1 = LoadImage("./resources/images/ast1_1.png");
    Image ast1_2 = LoadImage("./resources/images/ast1_2.png");
    Image ast1_3 = LoadImage("./resources/images/ast1_3.png");

    Image ast2   = LoadImage("./resources/images/ast2.png");
    Image ast2_1 = LoadImage("./resources/images/ast2_1.png");
    Image ast2_2 = LoadImage("./resources/images/ast2_2.png");
    Image ast2_3 = LoadImage("./resources/images/ast2_3.png");

    Image ast3   = LoadImage("./resources/images/ast3.png");
    Image ast3_1 = LoadImage("./resources/images/ast3_1.png");
    Image ast3_2 = LoadImage("./resources/images/ast3_2.png");
    Image ast3_3 = LoadImage("./resources/images/ast3_3.png");
    
    Image ast4   = LoadImage("./resources/images/ast4.png");
    Image ast4_1 = LoadImage("./resources/images/ast4_1.png");
    Image ast4_2 = LoadImage("./resources/images/ast4_2.png");
    Image ast4_3 = LoadImage("./resources/images/ast4_3.png");

    // get pixel data from images
    Color* torp_pix = LoadImageColors(torp);

    Color* ast1_pix   = LoadImageColors(ast1);
    Color* ast1_1_pix = LoadImageColors(ast1_1);
    Color* ast1_2_pix = LoadImageColors(ast1_2);
    Color* ast1_3_pix = LoadImageColors(ast1_3);
    
    Color* ast2_pix   = LoadImageColors(ast2);
    Color* ast2_1_pix = LoadImageColors(ast2_1);
    Color* ast2_2_pix = LoadImageColors(ast2_2);
    Color* ast2_3_pix = LoadImageColors(ast2_3);
    
    Color* ast3_pix   = LoadImageColors(ast3);
    Color* ast3_1_pix = LoadImageColors(ast3_1);
    Color* ast3_2_pix = LoadImageColors(ast3_2);
    Color* ast3_3_pix = LoadImageColors(ast3_3);
    
    Color* ast4_pix   = LoadImageColors(ast4);
    Color* ast4_1_pix = LoadImageColors(ast4_1);
    Color* ast4_2_pix = LoadImageColors(ast4_2);
    Color* ast4_3_pix = LoadImageColors(ast4_3);

    // load texture from images
    Texture2D tex_ship = LoadTextureFromImage(ship_img);
    Texture2D tex_torp = LoadTextureFromImage(torp);
    Texture2D tex_ufo  = LoadTextureFromImage(ufo);

    Texture2D tex_ast1   = LoadTextureFromImage(ast1);
    Texture2D tex_ast1_1 = LoadTextureFromImage(ast1_1);
    Texture2D tex_ast1_2 = LoadTextureFromImage(ast1_2);
    Texture2D tex_ast1_3 = LoadTextureFromImage(ast1_3);
    
    Texture2D tex_ast2   = LoadTextureFromImage(ast2);
    Texture2D tex_ast2_1 = LoadTextureFromImage(ast2_1);
    Texture2D tex_ast2_2 = LoadTextureFromImage(ast2_2);
    Texture2D tex_ast2_3 = LoadTextureFromImage(ast2_3);
    
    Texture2D tex_ast3   = LoadTextureFromImage(ast3);
    Texture2D tex_ast3_1 = LoadTextureFromImage(ast3_1);
    Texture2D tex_ast3_2 = LoadTextureFromImage(ast3_2);
    Texture2D tex_ast3_3 = LoadTextureFromImage(ast3_3);
    
    Texture2D tex_ast4 = LoadTextureFromImage(ast4);
    Texture2D tex_ast4_1 = LoadTextureFromImage(ast4_1);
    Texture2D tex_ast4_2 = LoadTextureFromImage(ast4_2);
    Texture2D tex_ast4_3 = LoadTextureFromImage(ast4_3);

    Color* ast_pixels[16]  = {
        ast1_pix, ast1_1_pix, ast1_2_pix, ast1_3_pix,
        ast2_pix, ast2_1_pix, ast2_2_pix, ast2_3_pix,
        ast3_pix, ast3_1_pix, ast3_2_pix, ast3_3_pix,
        ast4_pix, ast4_1_pix, ast4_2_pix, ast4_3_pix
    };

    Texture2D ast_sprites[16] = {
        tex_ast1, tex_ast1_1, tex_ast1_2, tex_ast1_3,
        tex_ast2, tex_ast2_1, tex_ast2_2, tex_ast2_3,
        tex_ast3, tex_ast3_1, tex_ast3_2, tex_ast3_3,
        tex_ast4, tex_ast4_1, tex_ast4_2, tex_ast4_3
    };

    ///////////////////////////////////////////////////////

    ship.tex = tex_ship;
    ship.pix = LoadImageColors(ship_img);
    // ship.tex.width *= scale;
    // ship.tex.height *= scale;
    ship.pos = (Vector2){screen_width/2, screen_height/2};
    ship.size = (Vector2){ship.tex.width, ship.tex.height};
    ship.center = (Vector2){ship.tex.width/2, ship.tex.height/2};
    ship.rect = (Rectangle){0, 0, ship.tex.width, ship.tex.height};
    ship.bounds = (Rectangle){ship.pos.x, ship.pos.y, ship.size.x, ship.size.y};
    // ship.bounds = (Rectangle){ship.pos.x-ship.center.x, ship.pos.y-ship.center.y, ship.size.x, ship.size.y}; // offset bbox
    // ship.rotation = -90;
    // ship.circle_center = (Vector2){ship.pos.x-ship.tex.width/2, ship.pos.y-ship.tex.height/2};
    ship.radius = ship.tex.width / 3.33f;
    // ship.scale = 0.5;
    ship.lowSpeed = 0.004;
    ship.active = true;

    sluggo.pos = (Vector2){100, 100};

    // init asteroids
    Texture2D tex_ast;
    // Vector2 rotated_center;
    int x_pos;
    int y_pos;
    int spr_ind;
    int direction;
    // float size;
    float rotation;
    float rotated_center_x;
    float rotated_center_y;
    float radians;
    
    // size = 128 * ast_scale;

    for (i = 0; i < ast_num; i++) {
        x_pos = RandPos(-10, screen_width+10);
        y_pos = RandPos(-10, screen_height+10);
        rotation = ((float)rand() / RAND_MAX) * 360.0f;
        direction = rand() % 4;
        spr_ind = rand() % 16;
        tex_ast = ast_sprites[spr_ind];

        asteroids[i].tex = tex_ast;
        asteroids[i].pix = ast_pixels[spr_ind];
        // asteroids[i].tex.width *= scale;
        // asteroids[i].tex.height *= scale;
        asteroids[i].pos = (Vector2){x_pos, y_pos};
        asteroids[i].speed = (Vector2){1.7, 1.7};
        asteroids[i].size = (Vector2){128, 128};
        asteroids[i].center = (Vector2){asteroids[i].tex.width/2, asteroids[i].tex.height/2};
        asteroids[i].rect = (Rectangle){0, 0, asteroids[i].tex.width, asteroids[i].tex.height};
        // asteroids[i].rotation = rotation;
        asteroids[i].bounds = (Rectangle){asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].tex.width, asteroids[i].tex.height};
        asteroids[i].dir = direction;
        asteroids[i].scale = 1.0;
        asteroids[i].active = true;
        asteroids[i].radius = asteroids[i].tex.width / 2.0f;
    }   

    // init torps
    int torp_index = 0;
    for (i = 0; i < 4; i++) {
        torps[i].tex = tex_torp;
        torps[i].pix = LoadImageColors(torp);
        torps[i].active = false;
        torps[i].speed = (Vector2){0.5, 0.5};
        torps[i].size = (Vector2){4, 4};
        torps[i].source = (Rectangle){0, 0, torps[i].tex.width, torps[i].tex.height};
        torps[i].center = (Vector2){torps[i].pos.x+torps[i].tex.width/2, torps[i].pos.y+torps[i].tex.height/2};
        torps[i].radius = torps[i].tex.width / 1.7;
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

    //////////////////////////////////////////// GAME LOOP //////////////////////////////////////////

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // moves ship with mouse for testing collisions
        HideCursor();
        mouse_pos = GetMousePosition();
        ship.pos = (Vector2){mouse_pos.x-ship.tex.width/2, mouse_pos.y-ship.tex.height/2};

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

        // /////////////// SHOOTING ///////////////

        if (IsKeyPressed(KEY_LEFT_CONTROL)) {
            if (torp_index == 4) {
                // if torp_index==3: timer, then i=0
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
                torps[i].dest = (Rectangle){torps[i].pos.x, torps[i].pos.y, torps[i].tex.width, torps[i].tex.height};
                DrawTexturePro(tex_torp, torps[i].source, torps[i].dest, torps[i].center, 0.0f, WHITE);
            }
            // wrap-around
            if (torps[i].pos.x-torps[i].tex.width >= screen_width)
                torps[i].pos.x = 0 - torps[i].tex.width;
            else if (torps[i].pos.x+torps[i].tex.width <= 0)
                torps[i].pos.x = screen_width + torps[i].tex.width;

            if (torps[i].pos.y-torps[i].tex.height >= screen_height)
                torps[i].pos.y = 0 - torps[i].tex.height;
            else if (torps[i].pos.y+torps[i].tex.height <= 0)
                torps[i].pos.y = screen_height + torps[i].tex.height;
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

        ship.circle_center = (Vector2){ship.pos.x-ship.tex.width/2, ship.pos.y-ship.tex.height/2};

        /////////////// SHIP AND ASTEROIDS MOVEMENT ///////////////
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
            // switch(asteroids[i].dir) {
            //     case 0:
            //         asteroids[i].pos.x += ast_speed;
            //         asteroids[i].pos.y += ast_speed;
            //         break;
            //     case 1:
            //         asteroids[i].pos.x -= ast_speed;
            //         asteroids[i].pos.y -= ast_speed;
            //         break;
            //     case 2:
            //         asteroids[i].pos.x += ast_speed;
            //         asteroids[i].pos.y -= ast_speed;
            //         break;
            //     case 3:
            //         asteroids[i].pos.x -= ast_speed;
            //         asteroids[i].pos.y += ast_speed;
            //         break;
            // }

            // asteroids[i].bounds.x = asteroids[i].pos.x;
            // asteroids[i].bounds.y = asteroids[i].pos.y;
        }

        // draw asteroids
        for (i = 0; i < ast_num; i++) {
            if (asteroids[i].active) 
                DrawTexturePro(asteroids[i].tex, asteroids[i].rect, asteroids[i].bounds, asteroids[i].center, asteroids[i].rotation, RAYWHITE);
        }

        // draw ship and ufos
        if (ship.active) 
            DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);

        /////////////// COLLISIONS ///////////////

        // asteroids vs ship (simple cirle collisions)
        for (i = 0; i < ast_num; i++) {
            // float rotation_radians = asteroids[i].rotation * DEG2RAD;
            Vector2 asteroid_top_left = { asteroids[i].pos.x - asteroids[i].tex.width / 2,
                              asteroids[i].pos.y - asteroids[i].tex.height / 2 };

            // Vector2 ship_top_left = { ship.pos.x - ship.tex.width,
            //                         ship.pos.y - ship.tex.height};

            if (CheckCollisionCircles(asteroids[i].pos, asteroids[i].radius, ship.circle_center, ship.radius)) {
                // collision_found = false;
                DrawText("Collision!", 10, 50, 40, RED);

                // for (int y = 0; y < asteroids[i].tex.height && !collision_found; y++) {
                //     for (int x = 0; x < asteroids[i].tex.width && !collision_found; x++) {
                //         int ast_local_x  = x;
                //         int ast_local_y  = y;
                //         // int ship_local_x = (x + (int)(asteroids[i].pos.x - ship.pos.x));
                //         // int ship_local_y = (y + (int)(asteroids[i].pos.y - ship.pos.y));
                //         int ship_local_x = (x + (int)(asteroid_top_left.x - ship_top_left.x));
                //         int ship_local_y = (y + (int)(asteroid_top_left.y - ship_top_left.y));

                //         if (ship_local_x>=0 && ship_local_y>=0 && ship_local_x < ship.tex.width && ship_local_y < ship.tex.height) {
                //             Color ast_pixel  = asteroids[i].pix[ast_local_y * asteroids[i].tex.width + ast_local_x];
                //             Color ship_pixel = ship.pix[ship_local_y * ship.tex.width + ship_local_x];

                //             if (ast_pixel.a > 0 && ship_pixel.a > 0) {
                //                 collision_found = true;
                //                 DrawText("Collision!", 10, 50, 40, RED);
                //                 // asteroids[i].active = false;
                //                 // ship.active = false;
                //             }  
                //         }
                //     }
                // }
            }

            // asteroids vs torps (pixel-perfect using alpha channels)
            // for (j = 0; j < 4; j++) {
            //      Vector2 torp_top_left = {torps[j].pos.x - torps[j].tex.width, torps[j].pos.y - torps[j].tex.height};
            //     // if (asteroids[i].active && torps[j].active) {
            //         if (CheckCollisionCircles(asteroids[i].pos, asteroids[i].radius, torps[j].pos, torps[j].radius)) {
            //             collision_found = false;
            //             DrawText("1st Check", 10, 20, 30, GREEN);
            //             for (int y = 0; y < asteroids[i].tex.height; y++) {
            //                 for (int x = 0; x < asteroids[i].tex.width; x++) {
            //                     int ast_local_x  = (x + (int)(asteroids[i].pos.x - torps[j].pos.x));
            //                     int ast_local_y  = (y + (int)(asteroids[i].pos.y - torps[j].pos.y));
            //                     int torp_local_x = (x + (int)(asteroid_top_left.x - torps[j].pos.x));
            //                     int torp_local_y = (y + (int)(asteroid_top_left.y - torps[j].pos.y));
            //                     // int torp_local_x = (x + (int)(asteroid_top_left.x - torp_top_left.x));
            //                     // int torp_local_y = (y + (int)(asteroid_top_left.y - torp_top_left.y));

            //                     if (torp_local_x>=0 && torp_local_y>=0 && torp_local_x<torps[j].tex.width && torp_local_y<torps[j].tex.height) {
            //                         Color ast_pixel  = asteroids[i].pix[ast_local_y * asteroids[i].tex.width + ast_local_x];
            //                         Color torp_pixel = torps[j].pix[torp_local_y * torps[j].tex.width + torp_local_x];

            //                         if (ast_pixel.a > 0 && torp_pixel.a > 0) {
            //                             collision_found = true;
            //                             DrawText("Coliision!", 10, 50, 40, RED);
            //                             // asteroids[i].active = false;
            //                             // torps[j].active = false;
            //                         }
            //                     }
            //                 }
            //             }
            //         }
            //     // }
            // }

            for (j = 0; j < 4; j++) {
                 Vector2 torp_top_left = {torps[j].pos.x - torps[j].tex.width, torps[j].pos.y - torps[j].tex.height};
                if (asteroids[i].active && torps[j].active) {
                    if (CheckCollisionCircles(asteroids[i].pos, asteroids[i].radius, torps[j].pos, torps[j].radius)) {
                        collision_found = false;
                        DrawText("1st Check", 10, 20, 30, GREEN);
                        for (int y = 0; y < asteroids[i].tex.height && !collision_found; y++) {
                            for (int x = 0; x < asteroids[i].tex.width && !collision_found; x++) {
                                int ast_local_x  = (x + (int)(asteroids[i].pos.x - torps[j].pos.x));
                                int ast_local_y  = (y + (int)(asteroids[i].pos.y - torps[j].pos.y));
                                int torp_local_x = (x + (int)(asteroids[i].pos.x - torp_top_left.x));
                                int torp_local_y = (y + (int)(asteroids[i].pos.y - torp_top_left.y));
                                // int torp_local_x = (x + (int)(asteroid_top_left.x - torp_top_left.x));
                                // int torp_local_y = (y + (int)(asteroid_top_left.y - torp_top_left.y));

                                if (torp_local_x >= 0 && torp_local_y >= 0 
                                    && torp_local_x < torps[j].tex.width && torp_local_y < torps[j].tex.height) {

                                    Color ast_pixel  = asteroids[i].pix[ast_local_y * asteroids[i].tex.width + ast_local_x];
                                    Color torp_pixel = torps[j].pix[torp_local_y * torps[j].tex.width + torp_local_x];

                                    if (ast_pixel.a > 0 && torp_pixel.a > 0) {
                                        collision_found = true;
                                        DrawText("Coliision!", 10, 50, 40, RED);
                                        // asteroids[i].active = false;
                                        // torps[j].active = false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /////////////// DEBUGGING DISPLAY ///////////////
        if (debug) {
            DrawText(TextFormat("Rotation: %.2f", ship.rotation), 20, screen_height-90, 20, WHITE);
            DrawText(TextFormat("PosX: %.2f, ", ship.pos.x), 20, screen_height-70, 20, WHITE);
            DrawText(TextFormat("PosY: %.2f", ship.pos.y), 180, screen_height-70, 20, WHITE);
            // DrawText(TextFormat("Thrust: %.2f", thrust), 20, screen_height-50, 20, WHITE);   // test 
            // DrawText(TextFormat("Vel: %.2f, ", ship.vel.x), 20, screen_height-30, 20, WHITE);  // test
            // DrawText(TextFormat("Accel X: %.2f", ship.accel.x), 160, screen_height-30, 20, WHITE);         // test

            // show bounding circles
            if (ship.active)
                DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius, GREEN);
            
            for (i = 0; i < ast_num; i++) {
                if (asteroids[i].active)
                    DrawCircleLines(asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].radius, ORANGE);
                    DrawCircle(asteroids[i].pos.x, asteroids[i].pos.y, 3, GREEN);
            }
            
            for (i = 0; i < 4; i++) {
                if (torps[i].active)
                    DrawCircleLines(torps[i].pos.x, torps[i].pos.y, torps[i].radius, RED);
            } 

            DrawCircle(ship.pos.x, ship.pos.y, 3.0, ORANGE);
        }

        EndDrawing();
    }

    // unload images
    UnloadImage(ship_img);
    UnloadImage(torp);
    UnloadImage(ufo);

    UnloadImage(ast1);
    UnloadImage(ast1_1);
    UnloadImage(ast1_2);
    UnloadImage(ast1_3);

    UnloadImage(ast2);
    UnloadImage(ast2_1);
    UnloadImage(ast2_2);
    UnloadImage(ast2_3);

    UnloadImage(ast3);
    UnloadImage(ast3_1);
    UnloadImage(ast3_2);
    UnloadImage(ast3_3);

    UnloadImage(ast4);
    UnloadImage(ast4_1);
    UnloadImage(ast4_2);
    UnloadImage(ast4_3);

    // free all pixel data
    UnloadImageColors(ship.pix);
    UnloadImageColors(torp_pix);

    UnloadImageColors(ast1_pix);
    UnloadImageColors(ast1_1_pix);
    UnloadImageColors(ast1_2_pix);
    UnloadImageColors(ast1_3_pix);

    UnloadImageColors(ast2_pix);
    UnloadImageColors(ast2_1_pix);
    UnloadImageColors(ast2_2_pix);
    UnloadImageColors(ast2_3_pix);

    UnloadImageColors(ast3_pix);
    UnloadImageColors(ast3_1_pix);
    UnloadImageColors(ast3_2_pix);
    UnloadImageColors(ast3_3_pix);

    UnloadImageColors(ast4_pix);
    UnloadImageColors(ast4_1_pix);
    UnloadImageColors(ast4_2_pix);
    UnloadImageColors(ast4_3_pix);

    CloseWindow();

    return 0;
}

int RandPos(int a, int b)
{
    return a + rand() % (b - a+1);
}