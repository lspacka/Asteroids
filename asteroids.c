//  -lraylib -lgdi32 -lwinmm -Wall -std=c99 -I c:/raylib/raylib/src

#include "raylib.h"
#include "raymath.h"
// #include "synth.h"
// #include "animations.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

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
    float scale;        //
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
    char* type;
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

typedef struct Timer {
    double startTime;
    double lifeTime;
} Timer;

int RandPos(int a, int b);
void startTimer(Timer* timer, double lifetime);
bool TimerDone(Timer timer);
double getElapsed(Timer timer);
void AstBlast(Asteroid ast, Asteroid* asts, int* index);

int main()
{
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));

    int screen_width = 900;
    int screen_height = 900;

    InitWindow(screen_width, screen_height, "Asteroids");
    SetTargetFPS(60);

    const float rotation_speed = 2.5f;
    const float ufo_speed = 0.9;
    const float ast_scale =  0.703125;
    const float ast_speed = 1.7;  // 1.5
    // const float scale = 0.7f;
    int ast_num = 4;
    int mid_ast_num = ast_num * 2;
    int lil_ast_num = mid_ast_num * 2;
    int all_asts_num = ast_num * mid_ast_num * lil_ast_num;

    int mid_ast_ind = 0;
    int lil_ast_ind = 0;
    int* mid_ast_ptr = &mid_ast_ind;
    int* lil_ast_ptr = &lil_ast_ind; 

    int i, j, k;
    bool debug = false;
    bool collision_found = false;
    Vector2 mouse_pos;

    Ship ship = { 0 };
    UFO sluggo = { 0 };
    UFO mr_bill = { 0 };
    // Asteroid asteroids[4] = { 0 };
    Asteroid* asteroids = (Asteroid*)calloc(ast_num, sizeof(Asteroid));
    Asteroid* mid_asts  = (Asteroid*)calloc(mid_ast_num, sizeof(Asteroid));
    Asteroid* lil_asts  = (Asteroid*)calloc(lil_ast_num, sizeof(Asteroid));
    Asteroid** all_asts  = (Asteroid**)calloc((ast_num*mid_ast_num*lil_ast_num), sizeof(Asteroid));
    
    // all_asts[0] = asteroids;
    // all_asts[1] = mid_asts;
    // all_asts[2] = lil_asts;
    Torp torps[4] = { 0 };

    /////////////// IMAGES AND TEXTURES SETUP ///////////////

    Image ship_img = LoadImage("./resources/images/ship.png");
    Image torp     = LoadImage("./resources/images/torp2.png");
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
    ship.tex.width *= 0.9;
    ship.tex.height *= 0.9;
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
    float check1_radius = 1.4f;     // 1st check for torps collision.  1.2f

    for (i = 0; i < ast_num; i++) {
        x_pos = RandPos(-10, screen_width+10);
        y_pos = RandPos(-10, screen_height+10);
        // rotation = ((float)rand() / RAND_MAX) * 360.0f;
        direction = rand() % 4;
        spr_ind = rand() % 16;
        tex_ast = ast_sprites[spr_ind];

        asteroids[i].type = "big";
        asteroids[i].tex = tex_ast;
        asteroids[i].pix = ast_pixels[spr_ind];
        // asteroids[i].tex.width *= scale;
        // asteroids[i].tex.height *= scale;
        asteroids[i].pos = (Vector2){x_pos, y_pos};
        asteroids[i].speed = (Vector2){1.7, 1.7};
        asteroids[i].size = (Vector2){asteroids[i].tex.width, asteroids[i].tex.height};
        asteroids[i].center = (Vector2){asteroids[i].tex.width/2, asteroids[i].tex.height/2};
        asteroids[i].rect = (Rectangle){0, 0, asteroids[i].tex.width, asteroids[i].tex.height};
        // asteroids[i].rotation = rotation;
        asteroids[i].bounds = (Rectangle){asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].tex.width, asteroids[i].tex.height};
        asteroids[i].dir = direction;
        asteroids[i].scale = 1.0;
        asteroids[i].active = true;
        asteroids[i].radius = asteroids[i].tex.width / 2.0f;

        for(j = 0; j < mid_ast_num; j++) {
            mid_asts[j].type = "mid";
            // setting the scaled sizes here so I can use them in the wrap-around logic for all asteroids
            mid_asts[j].tex = asteroids[i].tex;
            // mid_asts[j].tex.width *= 0.5;
            // mid_asts[j].tex.height *= 0.5;
        }

        for (k = 0; k < lil_ast_num; k++) {
            lil_asts[k].type = "lil";
            lil_asts[k].tex = asteroids[i].tex;
            // lil_asts[k].tex.width *= 0.25;
            // lil_asts[k].tex.height *= 0.25;
        }   
    }

    // for(i = 0; i < mid_ast_num; i++) {
    //     mid_asts[i].type = "mid";
    // }

    // for (i = 0; i < lil_ast_num; i++) {
    //     lil_asts[i].type = "lil";
    // }   

    all_asts[0] = asteroids;
    all_asts[1] = mid_asts;
    all_asts[2] = lil_asts;

    // init torps
    int torp_index = 0;
    for (i = 0; i < 4; i++) {   // should I give it a huge number instead of 4?
        torps[i].tex = tex_torp;
        torps[i].pix = LoadImageColors(torp);
        torps[i].active = false;
        torps[i].speed = (Vector2){11.0, 11.0};    // ideal = 10.0
        torps[i].size = (Vector2){4, 4};
        torps[i].source = (Rectangle){0, 0, torps[i].tex.width, torps[i].tex.height};
        torps[i].center = (Vector2){torps[i].pos.x+torps[i].tex.width/2, torps[i].pos.y+torps[i].tex.height/2};
        torps[i].radius = torps[i].tex.width / 1.7;
        // torps[i].tex.width *= 0.9;
        // torps[i].tex.height *= 0.9;
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

    Timer burst_timer = { 0 };
    Timer shot_timers[4] = { 0 };
    // Timer shot_timer  = { 0 };
    float burst_time  = 0.4f;
    float shot_time   = 1.5f;
    bool cooldown_active = false;

    //////////////////////////////////////////// GAME LOOP //////////////////////////////////////////

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // moves ship with mouse for testing collisions
        // HideCursor();
        // mouse_pos = GetMousePosition();
        // ship.pos = (Vector2){mouse_pos.x-ship.tex.width/2, mouse_pos.y-ship.tex.height/2};

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

        ///////////////// SHOOTING ///////////////
        if (IsKeyPressed(KEY_LEFT_CONTROL) && !cooldown_active) {
            // no burst timer
            if (torp_index == 4) 
                torp_index = 0;

            // start single shot timer
            startTimer(&shot_timers[torp_index], shot_time);  

            // update tip coords in torps before shooting
            torps[torp_index].pos = (Vector2){ship.tip.x-ship.size.x/2, ship.tip.y-ship.size.y/2};
            torps[torp_index].dir = (Vector2){ship.dir.x, ship.dir.y};
            torps[torp_index].active = true;

            torp_index++;
        }

        // burst timer (it dont feel right)
        // if (torp_index == 4 && !cooldown_active) {
        //     startTimer(&burst_timer, burst_time);
        //     cooldown_active = true;
        // }
        // if (cooldown_active && getElapsed(burst_timer) >= burst_time) {
        //     torp_index = 0;
        //     cooldown_active = false;
        // }

        // draw torps
        for (i = 0; i < 4; i++) {
            if (torps[i].active) {
                torps[i].pos.x += torps[i].dir.x * torps[i].speed.x;
                torps[i].pos.y += torps[i].dir.y * torps[i].speed.y;
                torps[i].dest = (Rectangle){torps[i].pos.x, torps[i].pos.y, torps[i].tex.width, torps[i].tex.height};
                DrawTexturePro(tex_torp, torps[i].source, torps[i].dest, torps[i].center, 0.0f, WHITE);

                // torp dissapears after 1.5 seconds
                if (getElapsed(shot_timers[i]) >= shot_time)
                    torps[i].active = false;
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

            // single shot timer. 1.5sec
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
        
        // ship wrap-around
        if (ship.pos.x-ship.size.x >= screen_width) 
            ship.pos.x = 0 - ship.size.x/2;
        else if (ship.pos.x+ship.size.x/2 <= 0)
            ship.pos.x = screen_width + ship.size.x;

        if (ship.pos.y-ship.size.y >= screen_height)
            ship.pos.y = 0 - ship.size.y/2;
        else if (ship.pos.y+ship.size.y/2 <= 0)
            ship.pos.y = screen_height + ship.size.y;

        ///////////// asteroids pos update and wrap-around logic /////////////

        // single wrap-around logic for all asteroids
        int ast_lim;
        for (i = 0; i < 3; i++) {
            if (i == 0)
                ast_lim = ast_num;
            else if (i == 1)
                ast_lim = mid_ast_num;
            else
                ast_lim = lil_ast_num;

            for (j = 0; j < ast_lim; j++) {
                if (all_asts[i][j].pos.x-all_asts[i][j].size.x/2 >= screen_width) 
                    all_asts[i][j].pos.x = 0 - all_asts[i][j].size.x/2;
                else if (all_asts[i][j].pos.x+all_asts[i][j].size.x/2 <= 0) 
                    all_asts[i][j].pos.x = screen_width + all_asts[i][j].size.x/2;

                if (all_asts[i][j].pos.y-all_asts[i][j].size.y/2 >= screen_height) 
                    all_asts[i][j].pos.y = 0 - all_asts[i][j].size.y/2;
                else if (all_asts[i][j].pos.y+all_asts[i][j].size.y/2 <= 0) 
                    all_asts[i][j].pos.y = screen_height +  all_asts[i][j].size.y/2;
            }
        }

        // POSITION UPDATE
        // BIG
        for (i = 0; i < ast_num; i++) {
            // wrap-around
            // if (asteroids[i].pos.x-asteroids[i].size.x/2 >= screen_width) 
            //     asteroids[i].pos.x = 0 - asteroids[i].size.x/2;
            // else if (asteroids[i].pos.x+asteroids[i].size.x/2 <= 0) 
            //     asteroids[i].pos.x = screen_width + asteroids[i].size.x/2;

            // if (asteroids[i].pos.y-asteroids[i].size.y/2 >= screen_height) 
            //     asteroids[i].pos.y = 0 - asteroids[i].size.y/2;
            // else if (asteroids[i].pos.y+asteroids[i].size.y/2 <= 0) 
            //     asteroids[i].pos.y = screen_height +  asteroids[i].size.y/2;

            switch(asteroids[i].dir) {
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
            case 0:
                        asteroids[i].pos.x -= ast_speed;
                        asteroids[i].pos.y -= ast_speed;
                        break;
                    case 1:
                        asteroids[i].pos.x += ast_speed;
                        asteroids[i].pos.y -= ast_speed;
                        break;
                    case 2:
                        asteroids[i].pos.x -= ast_speed;
                        asteroids[i].pos.y += ast_speed;
                        break;
                    case 3:
                        asteroids[i].pos.x += ast_speed;
                        asteroids[i].pos.y += ast_speed;
                        break;
                }
            asteroids[i].bounds.x = asteroids[i].pos.x;
            asteroids[i].bounds.y = asteroids[i].pos.y;
        }

        // MID
        for (i = 0; i < mid_ast_num; i++) {
            if (mid_asts[i].active) {
                switch(mid_asts[i].dir) {
                    case 0:
                        mid_asts[i].pos.x -= ast_speed;
                        mid_asts[i].pos.y -= ast_speed;
                        break;
                    case 1:
                        mid_asts[i].pos.x += ast_speed;
                        mid_asts[i].pos.y -= ast_speed;
                        break;
                    case 2:
                        mid_asts[i].pos.x -= ast_speed;
                        mid_asts[i].pos.y += ast_speed;
                        break;
                    case 3:
                        mid_asts[i].pos.x += ast_speed;
                        mid_asts[i].pos.y += ast_speed;
                        break;
                }
                mid_asts[i].bounds.x = mid_asts[i].pos.x;
                mid_asts[i].bounds.y = mid_asts[i].pos.y;
            }
        }

        // LIL
        for (i = 0; i < lil_ast_num; i++) {
            if (lil_asts[i].active) {
                switch(lil_asts[i].dir) {
                    case 0:
                        lil_asts[i].pos.x -= ast_speed;
                        lil_asts[i].pos.y -= ast_speed;
                        break;
                    case 1:
                        lil_asts[i].pos.x += ast_speed;
                        lil_asts[i].pos.y -= ast_speed;
                        break;
                    case 2:
                        lil_asts[i].pos.x -= ast_speed;
                        lil_asts[i].pos.y += ast_speed;
                        break;
                    case 3:
                        lil_asts[i].pos.x += ast_speed;
                        lil_asts[i].pos.y += ast_speed;
                        break;
                }
                lil_asts[i].bounds.x = lil_asts[i].pos.x;
                lil_asts[i].bounds.y = lil_asts[i].pos.y;
            }
        }

        ///////////// draw asteroids /////////////

        // BIG
        for (i = 0; i < ast_num; i++) {
            if (asteroids[i].active) 
                DrawTexturePro(asteroids[i].tex, asteroids[i].rect, asteroids[i].bounds, asteroids[i].center, asteroids[i].rotation, RAYWHITE);
        }

        // MID
        for (i = 0; i < mid_ast_num; i++) {
            if (mid_asts[i].active)
                DrawTexturePro(mid_asts[i].tex, mid_asts[i].rect, mid_asts[i].bounds, mid_asts[i].center, 0.0f, RAYWHITE);
        }

        // LIL
        for (i = 0; i < lil_ast_num; i++) {
            if (lil_asts[i].active)
                DrawTexturePro(lil_asts[i].tex, lil_asts[i].rect, lil_asts[i].bounds, lil_asts[i].center, 0.0f, RAYWHITE);
        }

        ///////////// draw ship and ufos /////////////
        if (ship.active) 
            DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);

        ////////////////////////// COLLISIONS //////////////////////////

        // big asteroids
        Vector2 asteroid_top_left;
        for (i = 0; i < ast_num; i++) {
            asteroid_top_left = (Vector2){ 
                asteroids[i].pos.x - asteroids[i].tex.width / 2,
                asteroids[i].pos.y - asteroids[i].tex.height / 2 
            };

            // vs ship (simple cirle collisions)
            if (asteroids[i].active && ship.active) {
                if (CheckCollisionCircles(asteroids[i].pos, asteroids[i].radius, ship.circle_center, ship.radius)) {
                    // collision_found = false;
                    DrawText("Collision!", 10, 50, 40, RED);
                    // asteroids[i].active = false;
                    // ship.active = false;
                }
            }

            // vs torps (pixel-perfect using alpha channels)
            for (j = 0; j < 4; j++) {
                if (asteroids[i].active && torps[j].active) {
                    if (CheckCollisionCircles(asteroids[i].pos, asteroids[i].radius*check1_radius, torps[j].pos, torps[j].radius)) {
                        collision_found = false;
                        // DrawText("1st Check", 10, 20, 30, GREEN);
                        for (int y = 0; y < asteroids[i].tex.height && !collision_found; y++) {
                            for (int x = 0; x < asteroids[i].tex.width && !collision_found; x++) {
                                int ast_local_x  = x;
                                int ast_local_y  = y;
                                int torp_local_x = (x + (int)(asteroid_top_left.x - torps[j].pos.x));
                                int torp_local_y = (y + (int)(asteroid_top_left.y - torps[j].pos.y));

                                if (torp_local_x >= 0 && torp_local_y >= 0 
                                    && torp_local_x < torps[j].tex.width && torp_local_y < torps[j].tex.height) {

                                    Color ast_pixel  = asteroids[i].pix[ast_local_y * asteroids[i].tex.width + ast_local_x];
                                    Color torp_pixel = torps[j].pix[torp_local_y * torps[j].tex.width + torp_local_x];

                                    if (ast_pixel.a > 0 && torp_pixel.a > 0) {
                                        collision_found = true;
                                        asteroids[i].active = false;
                                        torps[j].active = false;
                                        AstBlast(asteroids[i], mid_asts, mid_ast_ptr);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // mid asteroids
        // vs ship
        for (i = 0; i < mid_ast_num; i++) {
            if (mid_asts[i].active && ship.active) {
                if (CheckCollisionCircles(mid_asts[i].pos, mid_asts[i].radius, ship.circle_center, ship.radius)) {
                    DrawText("Collision!", 10, 50, 40, RED);
                    // mid_asts[i].active = false;
                    // ship.active = false;
                }
            }

            // vs torps
            for (j = 0; j < 4; j++) {
                if (mid_asts[i].active && torps[j].active) {
                    if (CheckCollisionCircles(mid_asts[i].pos, mid_asts[i].radius, torps[j].pos, torps[j].radius)) {
                        mid_asts[i].active = false;
                        torps[j].active = false;
                        AstBlast(mid_asts[i], lil_asts, lil_ast_ptr);
                    }
                }
            }
        }

        // lil asteroids
        // vs ship
        for (i = 0; i < lil_ast_num; i++) {
            if (lil_asts[i].active && ship.active) {
                if (CheckCollisionCircles(lil_asts[i].pos, lil_asts[i].radius, ship.circle_center, ship.radius)) {
                    DrawText("Collision!", 10, 50, 40, RED);
                    // lil_asts[i].active = false;
                    // ship.active = false;
                }
            }

            // vs torps
            for (j = 0; j < 4; j++) {
                if (lil_asts[i].active && torps[j].active) {
                    if (CheckCollisionCircles(lil_asts[i].pos, lil_asts[i].radius, torps[j].pos, torps[j].radius)) {
                        lil_asts[i].active = false;
                        torps[j].active = false;
                    }
                }
            }
        }

        /////////////// DEBUGGING DISPLAY ///////////////
        if (debug) {
            DrawText(TextFormat("Rotation: %.2f", ship.rotation), 20, screen_height-90, 20, WHITE);
            DrawText(TextFormat("PosX: %.2f, ", ship.pos.x), 20, screen_height-70, 20, WHITE);
            DrawText(TextFormat("PosY: %.2f", ship.pos.y), 180, screen_height-70, 20, WHITE);
            DrawText(TextFormat("torp_index: %d", torp_index), 20, screen_height-50, 20, WHITE);
            // DrawText(TextFormat("lil_ast_type: %s", lil_asts[lil_ast_num-1].type), 20, screen_height-30, 20, WHITE);
            // DrawText(TextFormat("lil_ast_ind: %d", lil_ast_ind), 20, screen_height-30, 20, WHITE);

            // show bounding circles
            if (ship.active)
                DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius, GREEN);
            
            // big asteroids
            for (i = 0; i < ast_num; i++) {
                if (asteroids[i].active) {
                    DrawCircleLines(asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].radius, ORANGE);              // vs ship collision
                    DrawCircleLines(asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].radius*check1_radius, RED);   // 1st check for torps
                    DrawCircle(asteroids[i].pos.x, asteroids[i].pos.y, 3, GREEN);                                      // center coords  
                }
                // DrawText(TextFormat("ast_type: %s", asteroids[0].type), 20, screen_height-30, 20, WHITE);
            }

            // mid asteroids
            for (i = 0; i < mid_ast_num; i++) {
                if (mid_asts[i].active) {
                    DrawCircleLines(mid_asts[i].pos.x, mid_asts[i].pos.y, mid_asts[i].radius, ORANGE);              
                    DrawCircle(mid_asts[i].pos.x, mid_asts[i].pos.y, 3, GREEN); 
                    // DrawText(TextFormat("ast_type: %s", mid_asts[0].type), 20, screen_height-30, 20, WHITE);                                     
                }
            }

            // lil asteroids
            for (i = 0; i < lil_ast_num; i++) {
                if (lil_asts[i].active) {
                    DrawCircleLines(lil_asts[i].pos.x, lil_asts[i].pos.y, lil_asts[i].radius, ORANGE);              
                    DrawCircle(lil_asts[i].pos.x, lil_asts[i].pos.y, 3, GREEN);
                    DrawText(TextFormat("ast_type: %s", lil_asts[0].type), 20, screen_height-30, 20, WHITE);                                       
                }
            }
            
            // torps
            for (i = 0; i < 4; i++) {
                if (torps[i].active)
                    DrawCircleLines(torps[i].pos.x, torps[i].pos.y, torps[i].radius, RED);
                    // DrawCircle(torps[i].pos.x, torps[i].pos.y, 1.0f, GREEN);
            } 

            // DrawCircle(ship.pos.x, ship.pos.y, 3.0, ORANGE);   
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

void AstBlast(Asteroid ast, Asteroid* asts, int* index)
{
    int i, direction, prev_dir = -1;

    for (i = 0; i < 2; i++) {
        do {
            direction = rand() % 4;
        } while (direction == ast.dir || (i == 1 && direction == prev_dir));

        prev_dir = direction;

        asts[*index].active = true;
        asts[*index].tex = ast.tex;

        if (strcmp(ast.type, "big") == 0) {
            asts[*index].type = "mid";
            asts[*index].tex.width *= 0.7;
            asts[*index].tex.height *= 0.7;
        } else if (strcmp(ast.type, "mid") == 0) {
            asts[*index].type = "lil";
            asts[*index].tex.width *= 0.5;
            asts[*index].tex.height *= 0.5;
        }

        asts[*index].pos = ast.pos;
        asts[*index].speed = ast.speed;
        asts[*index].size = (Vector2){asts[*index].tex.width, asts[*index].tex.height};
        asts[*index].center = (Vector2){asts[*index].size.x/2, asts[*index].size.y/2};
        asts[*index].rect = (Rectangle){0, 0, asts[*index].tex.width, asts[*index].tex.height};
        asts[*index].bounds = (Rectangle){asts[*index].pos.x, asts[*index].pos.y, asts[*index].tex.width, asts[*index].tex.height};
        asts[*index].dir = direction;
        asts[*index].radius = asts[*index].tex.width / 2.2f;

        (*index)++;
    }
}

void startTimer(Timer* timer, double lifetime)
{
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}

bool TimerDone(Timer timer)
{
    return GetTime() - timer.startTime >= timer.lifeTime;
}

double getElapsed(Timer timer)
{
    return GetTime() - timer.startTime;
}