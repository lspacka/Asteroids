//  -lraylib -lgdi32 -lwinmm -Wall -std=c99 -I c:/raylib/raylib/src

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"
#include "types.h"
#include "funcs.h"
// #include "synth.h"

void UpdateViewport(int width, int height) {
    SetWindowSize(width, height); // Updates Raylib's internal screen size
}

int main()
{
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));

    GameState game_state = GAME_INIT;
    bool first_level = true;
    // bool first_session = true;
    // Font my_font = LoadFont("D:/GitHub/C/raylib/asteroids/Hyperspace-JvEM.ttf");
    
    int screen_width = 1200;
    int screen_height = 900;

    // SetConfigFlags(FLAG_FULLSCREEN_MODE);
    // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "Asteroids"); 
    SetTargetFPS(60); 

    Vector2 spawn_point = { screen_width/2, screen_height/2 };   // to reset the ship pos to the middle of the screen
    const float rotation_speed = 2.5f;
    const float ufo_speed = 0.9;
    const float ast_scale =  0.703125;
    const int particle_number = 41;

    // asteroid quantities
    int ast_num;
    int mid_ast_num;
    int lil_ast_num;
    int all_asts_num;
    int active_asteroids;

    // asteroid indexes for AstBlast()
    int mid_ast_ind;
    int lil_ast_ind;
    int* mid_ast_ptr;
    int* lil_ast_ptr; 

    int i, j, k;
    int level = 1;
    int lives = 3;  // INIT
    int score = 0;  // INIT
    int high_score = 0; // INI
    bool debug = false;
    bool collision_found = false;
    bool hyperspace = false;
    bool player_dead = false;
    Vector2 mouse_pos;

    Ship ship = { 0 };
    UFO sluggo = { 0 };
    UFO mr_bill = { 0 };
    Torp torps[4] = { 0 };
    Torp ufo_torps[15] = { 0 };
    // Torp** all_torps[2];
    // Torp all_torps[[torps, ufo_torps]];
    Ship ships[3] = { 0 };
    Particle* particles = (Particle*)malloc(particle_number * sizeof(Particle));
    Stick sticks[5] = { 0 };

    sluggo.name = "sluggo";
    mr_bill.name = "mr bill";
    
    // INIT
    Asteroid* asteroids;
    Asteroid* mid_asts;
    Asteroid* lil_asts;
    Asteroid** all_asts;

    /////////////// IMAGES AND TEXTURES SETUP ///////////////

    Image ship_img  = LoadImage("./resources/images/ship.png");
    Image torp      = LoadImage("./resources/images/torp2.png");
    Image ufo_img   = LoadImage("./resources/images/UFO_2.png");
    Image stick_img = LoadImage("./resources/images/stick.png");

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
    Texture2D tex_ship   = LoadTextureFromImage(ship_img);
    Texture2D tex_torp   = LoadTextureFromImage(torp);
    Texture2D tex_ufo    = LoadTextureFromImage(ufo_img);
    Texture2D tex_stick  = LoadTextureFromImage(stick_img);

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
    
    // init UFOs
    int init_ufo_x;
    int init_ufo_y;

    sluggo.tex = tex_ufo;
    mr_bill.tex = tex_ufo;

    sluggo.tex.width *= 1.3;
    sluggo.tex.height *= 1.3;
    mr_bill.tex.width *= 0.7;
    mr_bill.tex.height *= 0.7;

    UFO ufos[2] = {sluggo, mr_bill};

    for (i = 0; i < 2; i++) {
        ufos[i].speed = (Vector2){2.5, 2.5};
        ufos[i].size = (Vector2){ufos[i].tex.width, ufos[i].tex.height};
        ufos[i].center = (Vector2){ufos[i].size.x/2, ufos[i].size.y/2};
        ufos[i].rect = (Rectangle){0, 0, ufos[i].size.x, ufos[i].size.y};
        ufos[i].bounds = (Rectangle){ufos[i].pos.x, ufos[i].pos.y, ufos[i].size.x, ufos[i].size.y};
        ufos[i].radius = ufos[i].size.x / 2.2f;
    }

    // UFO ufo_test = ufos[rand() % 2];

    // init ship
    ship.tex = tex_ship;
    ship.pix = LoadImageColors(ship_img);
    ship.tex.width *= 0.9;
    ship.tex.height *= 0.9;
    ship.size = (Vector2){ship.tex.width, ship.tex.height};
    ship.center = (Vector2){ship.size.x/2, ship.size.y/2};
    ship.rect = (Rectangle){0, 0, ship.size.x, ship.size.y};
    ship.bounds = (Rectangle){ship.pos.x, ship.pos.y, ship.size.x, ship.size.y};
    ship.radius = ship.tex.width / 3.33f;
    ship.lowSpeed = 0.004;
    // ship.active = true;

    // init asteroids
    Texture2D tex_ast;
    // Vector2 rotated_center;
    int x_pos;
    int y_pos;
    int spr_ind;
    int direction;
    int init_ast_pos;
    // float size;
    float rotation;
    float rotated_center_x;
    float rotated_center_y;
    float radians;
    float check1_radius = 1.4f;     // 1st check for torps collision.  1.2f
    float ast_speed = 0.0f;  // 1.5

    // init ship torps
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

    // init UFO torps
    int ufo_torp_index = 0;
    float ufo_torp_speed = 5.0f;
    for (i = 0; i < 15; i++) {  
        ufo_torps[i].tex = tex_torp;
        ufo_torps[i].pix = LoadImageColors(torp);
        ufo_torps[i].active = false;
        ufo_torps[i].speed = (Vector2){ufo_torp_speed, ufo_torp_speed};    // ideal = 10.0
        ufo_torps[i].size = (Vector2){4, 4};
        ufo_torps[i].source = (Rectangle){0, 0, ufo_torps[i].tex.width, ufo_torps[i].tex.height};
        ufo_torps[i].center = (Vector2){ufo_torps[i].pos.x+ufo_torps[i].tex.width/2, ufo_torps[i].pos.y+ufo_torps[i].tex.height/2};
        ufo_torps[i].radius = ufo_torps[i].tex.width / 1.7;
    }

    // init sticks
    for (i = 0; i < 5; i++) {
        sticks[i].tex = tex_stick;
        sticks[i].size = (Vector2){sticks[i].tex.width, sticks[i].tex.height};
        // sticks[i].center = (Vector2){sticks[i].size.x/2, sticks[i].size.y/2};
        sticks[i].rect = (Rectangle){0, 0, sticks[i].size.x, sticks[i].size.y};
        // sticks[i].bounds = (Rectangle){sticks[i].pos.x, sticks[i].pos.y, sticks[i].size.x, sticks[i].size.y};
        sticks[i].active = false;
    }

    // vars 4 trig calcs
    Vector2 thrust;
    Vector2 dir;    //
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

    // timers
    // Timer burst_timer = { 0 };
    // Timer shot_timers[4] = { 0 };
    Timer init_timer = { 0 };
    Timer hyper_timer = { 0 };
    Timer ship_spawn_timer = { 0 };
    // Timer ufo_shoot = { 0 };

    // float burst_time = 0.4f;
    float shot_time  = 1.5f;
    float hyper_time = 1.0f;
    float ship_spawn_time = 0.0f;
    bool cooldown_active = false;

    // UFO shooting vars
    Vector2 bill_shoot_dir = { 0 };
    float sluggo_angle = 0.0f;
    float bill_angle = 0.0f;
    float bill_new_angle = 0.0f;    // angle + deviation
    float max_deviation = 0.0f;
    float bill_random_deviation = 0.0f;
    int sluggo_dir = 0;
    int bill_deviation = 10;

    // Vector2 spawn_point = { screen_width/2, screen_height/2 };
    // startTimer(&init_timer, 5);

    ///////////////////////////////////////////////////////////// GAME LOOP /////////////////////////////////////////////////////////////

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        

        // toggle fullscreen
        if (IsKeyPressed(KEY_TAB)) {
            int display = GetCurrentMonitor();

            if (IsWindowFullscreen()) {
                // Exit fullscreen
                screen_width = 1200;
                screen_height = 900;
                ToggleFullscreen();
                SetWindowSize(screen_width, screen_height); 
            } else {
                // Enter fullscreen
                SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
                screen_width = GetMonitorWidth(display);
                screen_height = GetMonitorHeight(display);
                ToggleFullscreen();
            }

            // Update spawn point
            spawn_point = (Vector2){ (float)GetScreenWidth()/2, (float)GetScreenHeight()/2 };
        }

        // LIVES DISPLAY
        int life_pos_x = screen_width / 12;
        for (i = 0; i < lives; i++) {
            ships[i].active = true;
            ships[i].tex = tex_ship;
            ships[i].tex.width *= 0.7;
            ships[i].tex.height *= 0.7;
            ships[i].pos.x = life_pos_x;
            ships[i].pos.y = 100;
            life_pos_x += ships[i].tex.width;
        }

        // info display
        DrawText(TextFormat("%d", score), screen_width/10, 50, 40, RAYWHITE);
        DrawText(TextFormat("%d", high_score), screen_width/2, 50, 40, RAYWHITE);
        
        for (i = 0; i < lives; i++) 
            if (ships[i].active) 
                DrawTexture(ships[i].tex, ships[i].pos.x, ships[i].pos.y, WHITE);

        // moved ship movement and shooting code here
        // so it stays active during level progression
        // if (ship.active) 
        //     DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);

        if (IsKeyDown(KEY_RIGHT)) 
            ship.rotation += rotation_speed;
        if (IsKeyDown(KEY_LEFT))  
            ship.rotation -= rotation_speed;

        if (IsKeyDown(KEY_UP) && ship.active) {

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

        // wrap-around
        if (ship.pos.x-ship.size.x >= screen_width) 
            ship.pos.x = 0 - ship.size.x/2;
        else if (ship.pos.x+ship.size.x/2 <= 0)
            ship.pos.x = screen_width + ship.size.x;

        if (ship.pos.y-ship.size.y >= screen_height)
            ship.pos.y = 0 - ship.size.y/2;
        else if (ship.pos.y+ship.size.y/2 <= 0)
            ship.pos.y = screen_height + ship.size.y;

        // shooting
        // if (IsKeyPressed(KEY_LEFT_CONTROL) && !hyperspace && ship.active) {
        //     // no burst timer
        //     if (torp_index == 4) 
        //         torp_index = 0;

        //     // start single shot timer
        //     // startTimer(&shot_timers[torp_index], shot_time);  
        //     startTimer(&torps[torp_index].timer, shot_time);

        //     // update tip coords in torps before shooting
        //     torps[torp_index].pos = (Vector2){ship.tip.x-ship.size.x/2, ship.tip.y-ship.size.y/2};
        //     torps[torp_index].dir = (Vector2){ship.dir.x, ship.dir.y};
        //     torps[torp_index].active = true;

        //     torp_index++;
        // }

        // // draw ship torps
        // for (i = 0; i < 4; i++) {
        //     if (torps[i].active) {
        //         torps[i].pos.x += torps[i].dir.x * torps[i].speed.x;
        //         torps[i].pos.y += torps[i].dir.y * torps[i].speed.y;
        //         torps[i].dest = (Rectangle){torps[i].pos.x, torps[i].pos.y, torps[i].tex.width, torps[i].tex.height};
        //         DrawTexturePro(tex_torp, torps[i].source, torps[i].dest, torps[i].center, 0.0f, WHITE);

        //         // torp dissapears after 1.5 seconds
        //         // if (getElapsed(shot_timers[i]) >= shot_time)
        //         //     torps[i].active = false;
        //         if (TimerDone(torps[i].timer))
        //             torps[i].active = false;
        //     }

        //     // wrap-around
        //     if (torps[i].pos.x-torps[i].tex.width >= screen_width)
        //         torps[i].pos.x = 0 - torps[i].tex.width;
        //     else if (torps[i].pos.x+torps[i].tex.width <= 0)
        //         torps[i].pos.x = screen_width + torps[i].tex.width;

        //     if (torps[i].pos.y-torps[i].tex.height >= screen_height)
        //         torps[i].pos.y = 0 - torps[i].tex.height;
        //     else if (torps[i].pos.y+torps[i].tex.height <= 0)
        //         torps[i].pos.y = screen_height + torps[i].tex.height;
        // }

        switch(game_state) {
            case GAME_INIT: {
                if (IsKeyPressed(KEY_D))
                    debug = !debug;
                if (debug) 
                    DrawText(TextFormat("game state: %d", game_state), 20, screen_height-140, 20, RAYWHITE);

                if (first_level) {
                    // lives = 3;
                    // score = 0;
                    ast_num = 4;
                    ship.vel.x = 0;
                    ship.vel.y = 0;
                    ship.pos = (Vector2){screen_width/2, screen_height/2};
                    ship.active = true;
                    startTimer(&init_timer, 0.05);
                } else {
                    ast_num += 2;
                    startTimer(&init_timer, 1.5);
                }

                mid_ast_num = ast_num * 2;
                lil_ast_num = mid_ast_num * 2;
                all_asts_num = ast_num + mid_ast_num + lil_ast_num;
                active_asteroids = all_asts_num;

                if (first_level) {
                    asteroids = (Asteroid*)calloc(ast_num, sizeof(Asteroid));
                    mid_asts  = (Asteroid*)calloc(mid_ast_num, sizeof(Asteroid));
                    lil_asts  = (Asteroid*)calloc(lil_ast_num, sizeof(Asteroid));
                    all_asts  = (Asteroid**)calloc((all_asts_num), sizeof(Asteroid*));
                } else {
                    asteroids = (Asteroid*)realloc(asteroids, ast_num*sizeof(Asteroid));
                    mid_asts  = (Asteroid*)realloc(mid_asts, mid_ast_num*sizeof(Asteroid));
                    lil_asts  = (Asteroid*)realloc(lil_asts, lil_ast_num*sizeof(Asteroid));
                    all_asts  = (Asteroid**)realloc(all_asts, all_asts_num*sizeof(Asteroid*));
                }

                mid_ast_ind = 0;
                lil_ast_ind = 0;
                mid_ast_ptr = &mid_ast_ind;
                lil_ast_ptr = &lil_ast_ind; 

                // UFOs
                for (i = 0; i < 2; i++) {
                    startTimer(&ufos[i].respawnTimer, GetRandomValue(0, 10));
                    startTimer(&ufos[i].movementTimer, GetRandomValue(2, 5));
                    ufos[i].right = rand() % 2;

                    if (ufos[i].right)
                        init_ufo_x = screen_width + ufos[i].tex.width;
                    else 
                        init_ufo_x = 0 - ufos[i].tex.width;
                    init_ufo_y = RandPos(0, screen_height);

                    ufos[i].pos = (Vector2){init_ufo_x, init_ufo_y};
                    ufos[i].state = UFO_DEAD;
                    ufos[i].onscreen = false;
                    ufos[i].shoot = false;
                    // ufos[i].shoot_state = TRANSIT;
                    // ufos[i].active = true;      // ?
                }

                // ASTEROIDS
                for (i = 0; i < ast_num; i++) {
                    // set up a "safe zone" for the ship
                    // so that asteroids dont spawn right next to it
                    init_ast_pos = rand() % 2;
                    if (init_ast_pos == 0)
                        x_pos = RandPos(-10, (screen_width/2)-(ship.tex.width*4));
                    else
                        x_pos = RandPos((screen_width/2)+(ship.tex.width*4), screen_width+10);
                    y_pos = RandPos(-10, screen_height+10);

                    ast_speed = GetRandomFloat(1.5f, 2.5f);
                    direction = rand() % 4;
                    spr_ind = rand() % 16;
                    tex_ast = ast_sprites[spr_ind];

                    asteroids[i].type = "big";
                    asteroids[i].tex = tex_ast;
                    asteroids[i].pix = ast_pixels[spr_ind];
                    asteroids[i].pos = (Vector2){x_pos, y_pos};

                    asteroids[i].speed = (Vector2){ast_speed, ast_speed};
                    asteroids[i].size = (Vector2){asteroids[i].tex.width, asteroids[i].tex.height};
                    asteroids[i].center = (Vector2){asteroids[i].size.x/2, asteroids[i].size.y/2};
                    asteroids[i].rect = (Rectangle){0, 0, asteroids[i].size.x, asteroids[i].size.y};
                    asteroids[i].bounds = (Rectangle){asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].size.x, asteroids[i].size.y};
                    asteroids[i].dir = direction;
                    asteroids[i].scale = 1.0;
                    asteroids[i].active = true;
                    asteroids[i].radius = asteroids[i].size.x / 2.0f;
                    asteroids[i].active = true;

                    // setting temp textures here 
                    // so I can use them in the wrap-around logic for all asteroids
                    for(j = 0; j < mid_ast_num; j++) {
                        mid_asts[j].type = "mid";
                        mid_asts[j].tex = asteroids[i].tex;
                    }

                    for (k = 0; k < lil_ast_num; k++) {
                        lil_asts[k].type = "lil";
                        lil_asts[k].tex = asteroids[i].tex;
                    }
                }

                all_asts[0] = asteroids;
                all_asts[1] = mid_asts;
                all_asts[2] = lil_asts;

                ship.active = true;
                if (ship.active) 
                    DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);

                game_state = GAME_TRANSIT;
                break;
            }
            case GAME_TRANSIT: {
                if (ship.active) 
                    DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);

                if (TimerDone(init_timer)) {
                    // ship.active = true;
                    game_state = GAME_ACTIVE;
                    // break;
                }
                break;
            }
            case GAME_ACTIVE: {
                // move ship with mouse for testing collisions
                // HideCursor();
                // mouse_pos = GetMousePosition();
                // ship.pos = (Vector2){mouse_pos.x-ship.tex.width/2, mouse_pos.y-ship.tex.height/2};

                if (IsKeyPressed(KEY_D)) 
                    debug = !debug;

                // if (IsKeyDown(KEY_RIGHT)) 
                //     ship.rotation += rotation_speed;
                // if (IsKeyDown(KEY_LEFT))  
                //     ship.rotation -= rotation_speed;

                if (IsKeyDown(KEY_SPACE)) {
                    if (!hyperspace && ship.active) {
                        hyperspace = true;
                        ship.active = false;
                        startTimer(&hyper_timer, hyper_time);
                    }
                }

                if (hyperspace && TimerDone(hyper_timer)) {
                    ship.pos.x = RandPos(0, screen_width);
                    ship.pos.y = RandPos(0, screen_height);
                    ship.active = true;
                    hyperspace = false;
                }

                // calc ship's direction
                angle = ship.rotation * (PI/180.0);     // convert to radians
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
                if (IsKeyPressed(KEY_LEFT_CONTROL) && !hyperspace && ship.active) {
                    // no burst timer
                    if (torp_index == 4) 
                        torp_index = 0;

                    // start single shot timer
                    // startTimer(&shot_timers[torp_index], shot_time);  
                    startTimer(&torps[torp_index].timer, shot_time);

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

                // draw ship torps
                for (i = 0; i < 4; i++) {
                    if (torps[i].active) {
                        torps[i].pos.x += torps[i].dir.x * torps[i].speed.x;
                        torps[i].pos.y += torps[i].dir.y * torps[i].speed.y;
                        torps[i].dest = (Rectangle){torps[i].pos.x, torps[i].pos.y, torps[i].tex.width, torps[i].tex.height};
                        DrawTexturePro(tex_torp, torps[i].source, torps[i].dest, torps[i].center, 0.0f, WHITE);

                        // torp dissapears after 1.5 seconds
                        // if (getElapsed(shot_timers[i]) >= shot_time)
                        //     torps[i].active = false;
                        if (TimerDone(torps[i].timer))
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
                }

                /////////////// THRUST ///////////////
                // if (IsKeyDown(KEY_UP) && ship.active) {

                //     // Recalculate thrust direction based on the ship's current rotation
                //     thrust = ship.dir;
                //     Vector2Normalize(thrust);  // Normalize to get direction only

                //     // Apply thrust to velocity (incremental acceleration)
                //     ship.vel.x += thrust.x * thrust_force;
                //     ship.vel.y += thrust.y * thrust_force;

                //     // Optional: Cap the velocity to top speed
                //     speed = Vector2Length(ship.vel);
                //     if (speed > top_speed) {
                //         Vector2Normalize(ship.vel);
                //         ship.vel.x *= ship.lowSpeed;
                //         ship.vel.y *= ship.lowSpeed;
                //     }
                // } else {
                //     // No thrust, but apply slight friction to slow down gradually
                //     ship.vel.x *= friction;
                //     ship.vel.y *= friction;
                // }

                // // Update the ship's position based on velocity
                // ship.pos.x += ship.vel.x;
                // ship.pos.y += ship.vel.y;

                // // update ship's bounds to match the new position
                // ship.bounds.x = ship.pos.x - ship.center.x;
                // ship.bounds.y = ship.pos.y - ship.center.y;

                // ship.circle_center = (Vector2){ship.pos.x-ship.tex.width/2, ship.pos.y-ship.tex.height/2};

                /////////////// MOVEMENT LOGIC ///////////////
                
                // ship
                // for some reason, drawing twice (here or line 658)
                // somewhat remediates this ugly stuttering the ship has.
                // maybe Im just imagining it...
                if (ship.active) 
                    DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);
                
                // if (ship.active) 
                //     DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);
                // // wrap-around
                // if (ship.pos.x-ship.size.x >= screen_width) 
                //     ship.pos.x = 0 - ship.size.x/2;
                // else if (ship.pos.x+ship.size.x/2 <= 0)
                //     ship.pos.x = screen_width + ship.size.x;

                // if (ship.pos.y-ship.size.y >= screen_height)
                //     ship.pos.y = 0 - ship.size.y/2;
                // else if (ship.pos.y+ship.size.y/2 <= 0)
                //     ship.pos.y = screen_height + ship.size.y;

                // UFO
                bool anyUFOActive = false; 
                UFO* ufo = NULL;        // ?

                for (int i = 0; i < 2; i++) {
                    ufo = &ufos[i];
                    if (ufo->state != UFO_DEAD) {
                        anyUFOActive = true;
                        break;
                    }
                }

                int selectedUFO;
                // pick UFO and put it on hold
                if (!anyUFOActive) {
                    selectedUFO = rand() % 2; 
                    ufo = &ufos[selectedUFO];
                    startTimer(&ufo->respawnTimer, GetRandomValue(5, 10));  // time before 1st appearance
                    ufo->state = UFO_WAITING;
                    // set cooldown time before shooting
                    startTimer(&ufo->cooldownTimer, GetRandomValue(1, 2));
                    ufo->shoot_state = SHOOT_COOLDOWN;
                    ufo->onscreen = false;
                    sluggo_dir = 0;
                    sluggo_angle = 0;
                }

                // Process each UFO individually
                for (int i = 0; i < 2; i++) {
                    ufo = &ufos[i];

                    switch (ufo->state) {
                        case UFO_WAITING: {
                            if (TimerDone(ufo->respawnTimer)) {
                                ufo->state = UFO_SPAWNING;
                                ufo->right = rand() % 2;
                                ufo->pos.x = (ufo->right) ? screen_width + ufo->tex.width : 0 - ufo->tex.width;
                                ufo->pos.y = RandPos(0, screen_height);

                                startTimer(&ufo->movementTimer, GetRandomValue(1, 5));
                            }
                            break;
                        }
                        case UFO_SPAWNING: {
                            ufo->state = UFO_ACTIVE;
                            break;
                        }
                        case UFO_ACTIVE: {
                            if (ufo->pos.x > 0 || ufo->pos.x < screen_width)
                                ufo->onscreen = true;

                            if (TimerDone(ufo->movementTimer)) {
                                int deviation = GetRandomValue(-1, 1);
                                ufo->deviate = (deviation != 0); 
                                ufo->up = (deviation == -1);  

                                startTimer(&ufo->movementTimer, GetRandomValue(1, 5));
                            }

                            ufo->pos.x += (ufo->right ? -1 : 1) * ufo->speed.x;

                            if (ufo->deviate) 
                                ufo->pos.y += (ufo->up ? -1 : 1) * ufo->speed.y;

                            ufo->bounds.x = ufo->pos.x;
                            ufo->bounds.y = ufo->pos.y;

                            ////////// SHOOTING //////////
                            switch(ufo->shoot_state) {
                                case SHOOT_COOLDOWN: {
                                    if (TimerDone(ufo->cooldownTimer)) {
                                        // DrawText("enter COOLDOWN state...", 20, screen_height-90, 20, WHITE);
                                        ufo->shoot_state = SHOOT_TRANSIT;
                                        startTimer(&ufo->transitTimer, 0.1);
                                        // break;
                                    }            
                                    break;
                                }  
                                case SHOOT_TRANSIT: {
                                    if (TimerDone(ufo->transitTimer)) {
                                        // DrawText("enter TRANSIT state...", 300, screen_height-90, 20, WHITE);
                                        ufo->shoot_state = SHOOT_FIRE;
                                        startTimer(&ufo->transitTimer, 0.1);
                                        break;
                                    }
                                }
                                case SHOOT_FIRE: {
                                    // DrawText("enter SHOOT state...", 550, screen_height-90, 20, WHITE);
                                    if (TimerDone(ufo->transitTimer)) {
                                        if (strcmp(ufo->name, "sluggo") == 0) {
                                            sluggo_dir = GetRandomValue(0, 7);
                                            sluggo_dir *= 45;
                                            sluggo_angle = sluggo_dir * (PI/180.0);
                                            ufo->shootDir.x = cos(sluggo_angle);
                                            ufo->shootDir.y = sin(sluggo_angle);
                                        } else {
                                            if (ship.active) {
                                                max_deviation = bill_deviation * (PI/180.0);
                                                bill_random_deviation = GetRandomValue(-max_deviation, max_deviation);
                                                bill_shoot_dir = Vector2Subtract(ship.pos, ufo->pos);
                                                bill_angle = atan2(bill_shoot_dir.y, bill_shoot_dir.x);
                                                bill_new_angle = bill_angle + bill_random_deviation;
                                                ufo->shootDir.x = cos(bill_new_angle);
                                                ufo->shootDir.y = sin(bill_new_angle);
                                            }  // else shoot at random 
                                        }
                                        if (ufo_torp_index == 15)
                                            ufo_torp_index = 0;
            
                                        startTimer(&ufo_torps[ufo_torp_index].timer, shot_time);

                                        // update shooting coords 
                                        ufo_torps[ufo_torp_index].pos = (Vector2){ufo->pos.x, ufo->pos.y};
                                        ufo_torps[ufo_torp_index].dir = (Vector2){ufo->shootDir.x, ufo->shootDir.y};
                                        ufo_torps[ufo_torp_index].active = true;
                                        ufo_torp_index++;

                                        startTimer(&ufo->cooldownTimer, 1);
                                        ufo->shoot_state = SHOOT_COOLDOWN;
                                        break;
                                    }
                                }
                            }
                            /////////////////////////////

                            if (ufo->pos.x > screen_width + ufo->tex.width || 
                                ufo->pos.x < 0 - ufo->tex.width
                                // || !ufo->state==UFO_ACTIVE
                            ) {
                                ufo->state = UFO_DEAD;
                                ufo->onscreen = false;
                                ufo_torp_index = 0;
                            }
                            break;
                        }
                    }
                    
                    // adding ufo->onscreen fixes the bug 
                    // where the ufo flashes briefly on screen before appearing from the side
                    if (ufo->state == UFO_ACTIVE && ufo->onscreen) {        
                        DrawTexturePro(ufo->tex, ufo->rect, ufo->bounds, ufo->center, 0.0f, WHITE);
                    }

                    // draw UFO torps
                    for (int i = 0; i < 15; i++) {
                        if (ufo_torps[i].active) {
                            ufo_torps[i].pos.x += ufo_torps[i].dir.x * ufo_torps[i].speed.x;
                            ufo_torps[i].pos.y += ufo_torps[i].dir.y * ufo_torps[i].speed.y;
                            ufo_torps[i].dest = (Rectangle){ufo_torps[i].pos.x, ufo_torps[i].pos.y, ufo_torps[i].tex.width, ufo_torps[i].tex.height};
                            DrawTexturePro(tex_torp, ufo_torps[i].source, ufo_torps[i].dest, ufo_torps[i].center, 0.0f, WHITE);

                            // wrap-around
                            if (ufo_torps[i].pos.x-ufo_torps[i].tex.width >= screen_width)
                                ufo_torps[i].pos.x = 0 - ufo_torps[i].tex.width;
                            else if (ufo_torps[i].pos.x+ufo_torps[i].tex.width <= 0)
                                ufo_torps[i].pos.x = screen_width + ufo_torps[i].tex.width;

                            if (ufo_torps[i].pos.y-ufo_torps[i].tex.height >= screen_height)
                                ufo_torps[i].pos.y = 0 - ufo_torps[i].tex.height;
                            else if (ufo_torps[i].pos.y+ufo_torps[i].tex.height <= 0)
                                ufo_torps[i].pos.y = screen_height + ufo_torps[i].tex.height;

                            // torp dissapears after 1.5 seconds
                            if (TimerDone(ufo_torps[i].timer))
                                ufo_torps[i].active = false;
                        }
                    }

                    // UFO wrap-around
                    if (ufo->pos.y > screen_height+ufo->tex.height/2)
                        ufo->pos.y = 0 - ufo->tex.height/2;
                    else if (ufo->pos.y < 0-ufo->tex.height/2)
                        ufo->pos.y = screen_height + ufo->tex.height/2;

                    // UFO debug display
                    if (debug) {
                        DrawText("UFO: ", 20, screen_height-120, 20, WHITE);
                        // DrawText(TextFormat("shootDirX: %.2f,\tshootDirY: %.2f", ufo->shootDir.x, ufo->shootDir.y), 20, screen_height-90, 20, WHITE);
                        DrawText("shoot_state: ", 20, screen_height-70, 20, WHITE);
                        DrawText("UFO PosX: ", 20, screen_height-50, 20, WHITE);
                        DrawText("PosY: ", 220, screen_height-50, 20, WHITE);
                        DrawText(TextFormat("sluggo dir: ", sluggo_dir), 20, screen_height-30, 20, WHITE);
                        DrawText(TextFormat("angle: ", sluggo_angle), 220, screen_height-30, 20, WHITE);
                        // DrawText(TextFormat("UFO shoot: %d", ufo->shoot), 200, screen_height-30, 20, WHITE);
                        
                        if (ufo->state == UFO_ACTIVE) {
                            DrawText(TextFormat("%s", ufo->name), 72, screen_height-120, 20, WHITE);
                            // DrawText(TextFormat("shootDirX: %.2f,\tshootDirY: %.2f", ufo->shootDir.x, ufo->shootDir.y), 20, screen_height-90, 20, WHITE);
                            DrawText(TextFormat("torp index: %d", ufo_torp_index), 20, screen_height-90, 20, WHITE);
                            DrawText(TextFormat("%d", ufo->shoot_state), 172, screen_height-70, 20, WHITE);
                            DrawText(TextFormat("%.2f", ufo->pos.x), 135, screen_height-50, 20, WHITE);
                            DrawText(TextFormat("%.2f", ufo->pos.y), 290, screen_height-50, 20, WHITE);
                            DrawText(TextFormat("%d", sluggo_dir), 135, screen_height-30, 20, WHITE);
                            DrawText(TextFormat("%.2f", sluggo_angle), 290, screen_height-30, 20, WHITE);
                        }
                    }
                }

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
                    switch(asteroids[i].dir) {            
                        case 0:
                            asteroids[i].pos.x -= asteroids[i].speed.x;
                            asteroids[i].pos.y -= asteroids[i].speed.y;
                            break;
                        case 1:
                            asteroids[i].pos.x += asteroids[i].speed.x;
                            asteroids[i].pos.y -= asteroids[i].speed.y;
                            break;
                        case 2:
                            asteroids[i].pos.x -= asteroids[i].speed.x;
                            asteroids[i].pos.y += asteroids[i].speed.y;
                            break;
                        case 3:
                            asteroids[i].pos.x += asteroids[i].speed.x;
                            asteroids[i].pos.y += asteroids[i].speed.y;
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
                                mid_asts[i].pos.x -= mid_asts[i].speed.x;
                                mid_asts[i].pos.y -= mid_asts[i].speed.y;
                                break;
                            case 1:
                                mid_asts[i].pos.x += mid_asts[i].speed.x;
                                mid_asts[i].pos.y -= mid_asts[i].speed.y;
                                break;
                            case 2:
                                mid_asts[i].pos.x -= mid_asts[i].speed.x;
                                mid_asts[i].pos.y += mid_asts[i].speed.y;
                                break;
                            case 3:
                                mid_asts[i].pos.x += mid_asts[i].speed.x;
                                mid_asts[i].pos.y += mid_asts[i].speed.y;
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
                                lil_asts[i].pos.x -= lil_asts[i].speed.x;
                                lil_asts[i].pos.y -= lil_asts[i].speed.y;
                                break;
                            case 1:
                                lil_asts[i].pos.x += lil_asts[i].speed.x;
                                lil_asts[i].pos.y -= lil_asts[i].speed.y;
                                break;
                            case 2:
                                lil_asts[i].pos.x -= lil_asts[i].speed.x;
                                lil_asts[i].pos.y += lil_asts[i].speed.y;
                                break;
                            case 3:
                                lil_asts[i].pos.x += lil_asts[i].speed.x;
                                lil_asts[i].pos.y += lil_asts[i].speed.y;
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

                // draw explosion
                for (i = 0; i < particle_number; i++) {
                    if (particles[i].active) {
                        particles[i].pos.x += particles[i].speed * cosf(particles[i].angle);
                        particles[i].pos.y -= particles[i].speed * sinf(particles[i].angle);

                        if (TimerDone(particles[i].timer))
                            particles[i].active = false;
                    }
                }

                for (i = 0; i < particle_number; i++)
                    if (particles[i].active) 
                        DrawPixel((int)particles[i].pos.x, (int)particles[i].pos.y, WHITE);

                // draw disintegration
                for (i = 0; i < 5; i++) {
                    if (sticks[i].active) {
                        sticks[i].center.x += 0.2;  //(float)rand() / RAND_MAX - 0.3f
                        sticks[i].center.y += 0.2;  //(float)rand() / RAND_MAX - 0.3f
                    }

                    if (TimerDone(sticks[i].timer))
                        sticks[i].active = false;
                }

                for (i = 0; i < 5; i++) 
                    if (sticks[i].active)
                        DrawTexturePro(sticks[i].tex, sticks[i].rect, sticks[i].bounds, sticks[i].center, sticks[i].rotation, RAYWHITE);

                ///////////// draw ship and ufos /////////////
                // if (ship.active) 
                //     DrawTexturePro(ship.tex, ship.rect, ship.bounds, ship.center, ship.rotation, RAYWHITE);

                // if (ufo_test.active)
                //     DrawTexturePro(ufo_test.tex, ufo_test.rect, ufo_test.bounds, ufo_test.center, 0.0f, WHITE);

                ////////////////////////// COLLISIONS //////////////////////////

                // BIG ASTEROIDS
                Vector2 asteroid_top_left;
                for (i = 0; i < ast_num; i++) {
                    asteroid_top_left = (Vector2){ 
                        asteroids[i].pos.x - asteroids[i].tex.width / 2,
                        asteroids[i].pos.y - asteroids[i].tex.height / 2 
                    };

                    // vs ship
                    if (asteroids[i].active && ship.active) {
                        if (CheckCollisionCircles(asteroids[i].pos, asteroids[i].radius, ship.circle_center, ship.radius*1.7)) {
                            // collision_found = false;
                            // DrawText("Collision!", 10, 50, 40, RED);
                            // active_asteroids--;
                            // asteroids[i].active = false;
                            // AstBlast(asteroids[i], mid_asts, mid_ast_ptr, ast_sprites);
                            // desint(ship, sticks, &ship_spawn_time);
                            // pof(asteroids[i].pos, asteroids[i].radius, particles, particle_number);
                            // lives--;
                            // ships[lives].active = false;     // lives display
                            // ship.active = false;
                            // startTimer(&ship_spawn_timer, ship_spawn_time);
                        }
                    }
                    ShipSpawn(&ship, lives, asteroids, ast_num, ship_spawn_timer, spawn_point, &player_dead);

                    // vs ship torps (pixel-perfect using alpha channels)
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
                                                active_asteroids--;
                                                asteroids[i].active = false;
                                                torps[j].active = false;
                                                pof(asteroids[i].pos, asteroids[i].radius, particles, particle_number);
                                                AstBlast(asteroids[i], mid_asts, mid_ast_ptr, ast_sprites);
                                                score += 20;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // vs ufo torps
                    for (j = 0; j < 15; j++) {
                        if (asteroids[i].active && ufo_torps[j].active) {
                            if (CheckCollisionCircles(asteroids[i].pos, asteroids[i].radius*check1_radius, ufo_torps[j].pos, ufo_torps[j].radius)) {
                                collision_found = false;
                                // DrawText("1st Check", 10, 20, 30, GREEN);
                                for (int y = 0; y < asteroids[i].tex.height && !collision_found; y++) {
                                    for (int x = 0; x < asteroids[i].tex.width && !collision_found; x++) {
                                        int ast_local_x  = x;
                                        int ast_local_y  = y;
                                        int torp_local_x = (x + (int)(asteroid_top_left.x - ufo_torps[j].pos.x));
                                        int torp_local_y = (y + (int)(asteroid_top_left.y - ufo_torps[j].pos.y));

                                        if (torp_local_x >= 0 && torp_local_y >= 0 
                                            && torp_local_x < ufo_torps[j].tex.width && torp_local_y < ufo_torps[j].tex.height) {

                                            Color ast_pixel  = asteroids[i].pix[ast_local_y * asteroids[i].tex.width + ast_local_x];
                                            Color torp_pixel = ufo_torps[j].pix[torp_local_y * ufo_torps[j].tex.width + torp_local_x];

                                            if (ast_pixel.a > 0 && torp_pixel.a > 0) {  // !
                                                collision_found = true;
                                                // DrawText("UFO Collision!", 10, 50, 40, ORANGE);
                                                active_asteroids--;
                                                asteroids[i].active = false;
                                                ufo_torps[j].active = false;
                                                pof(asteroids[i].pos, asteroids[i].radius, particles, particle_number);
                                                AstBlast(asteroids[i], mid_asts, mid_ast_ptr, ast_sprites);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // vs ufos
                    for (k = 0; k < 2; k++) {
                        if (asteroids[i].active && ufos[k].state==UFO_ACTIVE && ufos[k].onscreen) {
                            if (CheckCollisionCircles(asteroids[i].pos, asteroids[i].radius, ufos[k].pos, ufos[k].radius)) {
                                // DrawText("UFO Collision!", 10, 50, 40, ORANGE);
                                active_asteroids--;
                                asteroids[i].active = false;
                                pof(asteroids[i].pos, asteroids[i].radius, particles, particle_number);
                                AstBlast(asteroids[i], mid_asts, mid_ast_ptr, ast_sprites);
                                ufos[k].state = UFO_DEAD;
                            }
                        }
                    }
                }

                // MID ASTEROIDS
                for (i = 0; i < mid_ast_num; i++) {
                    // vs ship
                    if (mid_asts[i].active && ship.active) {
                        if (CheckCollisionCircles(mid_asts[i].pos, mid_asts[i].radius, ship.circle_center, ship.radius)) {
                            // DrawText("Collision!", 10, 50, 40, RED);
                            // active_asteroids--;
                            // mid_asts[i].active = false;
                            // pof(mid_asts[i].pos, mid_asts[i].radius, particles, particle_number);
                            // desint(ship, sticks, &ship_spawn_time);
                            // AstBlast(mid_asts[i], lil_asts, lil_ast_ptr, ast_sprites);
                            // lives--;
                            // ship.active = false;
                            // ships[lives].active = false;
                            // startTimer(&ship_spawn_timer, ship_spawn_time);
                        }
                    }
                    ShipSpawn(&ship, lives, mid_asts, mid_ast_num, ship_spawn_timer, spawn_point, &player_dead);

                    // vs ship torps
                    for (j = 0; j < 4; j++) {
                        if (mid_asts[i].active && torps[j].active) {
                            if (CheckCollisionCircles(mid_asts[i].pos, mid_asts[i].radius, torps[j].pos, torps[j].radius)) {
                                active_asteroids--;
                                mid_asts[i].active = false;
                                torps[j].active = false;
                                pof(mid_asts[i].pos, mid_asts[i].radius, particles, particle_number);
                                AstBlast(mid_asts[i], lil_asts, lil_ast_ptr, ast_sprites);
                                score += 50;
                            }
                        }
                    }

                    // vs ufo torps
                    for (j = 0; j < 15; j++) {
                        if (mid_asts[i].active && ufo_torps[j].active) {
                            if (CheckCollisionCircles(mid_asts[i].pos, mid_asts[i].radius, ufo_torps[j].pos, ufo_torps[j].radius)) {
                                // DrawText("UFO Collision!", 10, 50, 40, ORANGE);
                                active_asteroids--;
                                mid_asts[i].active = false;
                                ufo_torps[j].active = false;
                                pof(mid_asts[i].pos, mid_asts[i].radius, particles, particle_number);
                                AstBlast(mid_asts[i], lil_asts, lil_ast_ptr, ast_sprites);
                            }
                        }
                    }

                    // vs ufos
                    for (k = 0; k < 2; k++) {
                        if (mid_asts[i].active && ufos[k].state==UFO_ACTIVE && ufos[k].onscreen) {
                            if (CheckCollisionCircles(mid_asts[i].pos, mid_asts[i].radius, ufos[k].pos, ufos[k].radius)) {
                                // DrawText("UFO Collision!", 10, 50, 40, ORANGE);
                                active_asteroids--;
                                mid_asts[i].active = false;
                                pof(mid_asts[i].pos, mid_asts[i].radius, particles, particle_number);
                                AstBlast(mid_asts[i], lil_asts, lil_ast_ptr, ast_sprites);
                                ufos[k].state = UFO_DEAD;
                            }
                        }
                    }
                }

                // LIL ASTEROIDS
                for (i = 0; i < lil_ast_num; i++) {
                    // vs ship
                    if (lil_asts[i].active && ship.active) {
                        if (CheckCollisionCircles(lil_asts[i].pos, lil_asts[i].radius, ship.circle_center, ship.radius)) {
                            // DrawText("Collision!", 10, 50, 40, RED);
                            // active_asteroids--;
                            // lil_asts[i].active = false;
                            // pof(lil_asts[i].pos, lil_asts[i].radius, particles, particle_number);
                            // desint(ship, sticks, &ship_spawn_time);
                            // lives--;
                            // ship.active = false;
                            // ships[lives].active = false;
                            // startTimer(&ship_spawn_timer, ship_spawn_time);
                        }
                    }
                    ShipSpawn(&ship, lives, lil_asts, lil_ast_num, ship_spawn_timer, spawn_point, &player_dead);

                    // vs ship torps
                    for (j = 0; j < 4; j++) {
                        if (lil_asts[i].active && torps[j].active) {
                            if (CheckCollisionCircles(lil_asts[i].pos, lil_asts[i].radius, torps[j].pos, torps[j].radius)) {
                                active_asteroids--;
                                lil_asts[i].active = false;
                                pof(lil_asts[i].pos, lil_asts[i].radius, particles, particle_number);
                                torps[j].active = false;
                                score += 100;
                            }
                        }
                    }

                    // vs ufo torps
                    for (j = 0; j < 15; j++) {
                        if (lil_asts[i].active && ufo_torps[j].active) {
                            if (CheckCollisionCircles(lil_asts[i].pos, lil_asts[i].radius, ufo_torps[j].pos, ufo_torps[j].radius)) {
                                // DrawText("Collision!", 10, 50, 40, RED);
                                active_asteroids--;
                                lil_asts[i].active = false;
                                pof(lil_asts[i].pos, lil_asts[i].radius, particles, particle_number);
                                ufo_torps[j].active = false;
                            }
                        }
                    }

                    // vs ufos
                    for (k = 0; k < 2; k++) {
                        if (lil_asts[i].active && ufos[k].state==UFO_ACTIVE && ufos[k].onscreen) {
                            if (CheckCollisionCircles(lil_asts[i].pos, lil_asts[i].radius, ufos[k].pos, ufos[k].radius)) {
                                // DrawText("UFO Collision!", 10, 50, 40, ORANGE);
                                active_asteroids--;
                                lil_asts[i].active = false;
                                pof(lil_asts[i].pos, lil_asts[i].radius, particles, particle_number);
                                ufos[k].state = UFO_DEAD;
                            }
                        }
                    }
                }

                // UFO
                for (i = 0; i < 2; i++) {
                    // vs ship
                    if (ufos[i].state==UFO_ACTIVE && ship.active) {
                        if (CheckCollisionCircles(ufos[i].pos, ufos[i].radius, ship.pos, ship.radius)) {
                            // DrawText("UFO Collision!", 10, 50, 40, ORANGE);
                            // ufos[i].state = UFO_DEAD;
                            // pof(ufos[i].pos, ufos[i].radius, particles, particle_number);
                            // desint(ship, sticks, &ship_spawn_time);
                            // lives--;
                            // ship.active = false;
                            // ships[lives].active = false;
                            // startTimer(&ship_spawn_timer, ship_spawn_time);
                        }
                    }
                    // ShipSpawn2(&ship, lives, ufos, 2, ship_spawn_timer, spawn_point, &game_state);

                    // vs torps
                    for (j = 0; j < 4; j++) {
                        if (CheckCollisionCircles(ufos[i].pos, ufos[i].radius, torps[j].pos, torps[j].radius) && 
                            torps[j].active && 
                            ufos[i].state == UFO_ACTIVE
                        ) {
                            // DrawText("Collision!", 10, 50, 40, RED);
                            if (strcmp(ufos[i].name, "sluggo") == 0)
                                score += 200;
                            else if (strcmp(ufos[i].name, "mr bill") == 0)
                                score += 1000;

                            torps[j].active = false;
                            ufos[i].state = UFO_DEAD;
                            pof(ufos[i].pos, ufos[i].radius, particles, particle_number);
                        }
                    }
                }

                // SHIP VS UFO TORPS
                for (i = 0; i < 15; i++) {
                    if (ufo_torps[i].active && ship.active) {
                        if (CheckCollisionCircles(ufo_torps[i].pos, ufo_torps[i].radius, ship.circle_center, ship.radius)) {
                            // DrawText("Collision!", 10, 50, 40, RED);
                            // ufo_torps[i].active = false;
                            // pof(ship.pos, ship.radius, particles, particle_number);
                            // desint(ship, sticks, &ship_spawn_time);
                            // ship.active = false;
                            // lives--;
                            // ships[lives].active = false;
                            // startTimer(&ship_spawn_timer, ship_spawn_time);
                        }
                    }
                }

                // level progression
                if (!player_dead && active_asteroids==0 && 
                    (!ufo->onscreen || 
                    ufo->state==UFO_DEAD || 
                    ufo->state==UFO_SPAWNING)) {
                    first_level = false;
                    level++;
                    game_state = GAME_INIT;
                }

                /////////////// DEBUGGING DISPLAY ///////////////
                
                if (debug) {
                    DrawText(TextFormat("game state: %d", game_state), 20, screen_height-200, 20, RAYWHITE);
                    DrawText(TextFormat("level: %d", level), 20, screen_height-180, 20, RAYWHITE);
                    DrawText(TextFormat("high score: %d", high_score), 20, screen_height-160, 20, RAYWHITE);
                    DrawText(TextFormat("asteroids: %d", ast_num), 20, screen_height-140, 20, RAYWHITE);
                    DrawText(TextFormat("all: %d", all_asts_num), 160, screen_height-140, 20, RAYWHITE);
                    DrawText(TextFormat("active: %d", active_asteroids), 260, screen_height-140, 20, RAYWHITE);
                    // DrawText(TextFormat("Lives left: %d", lives), 20, screen_height-110, 20, WHITE);
                    // DrawText(TextFormat("Rotation: %.2f", ship.rotation), 20, screen_height-110, 20, WHITE);
                    // DrawText(TextFormat("PosX: %.2f, ", ship.pos.x), 20, screen_height-90, 20, WHITE);
                    // DrawText(TextFormat("PosY: %.2f", ship.pos.y), 180, screen_height-90, 20, WHITE);
                    // DrawText("TEST", 20, screen_height-30, 20, WHITE);
                    
                    // SHOW BOUNDING CIRCLES
                    // ship
                    if (ship.active) {
                        DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius, GREEN);
                        DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius*1.7, ORANGE);
                        // safe zone for spawning:
                        DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius*14, RED);     
                        DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius*12, RED);
                        DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius*10, RED);
                        DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius*8, RED);     
                        DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius*6, RED);
                        DrawCircleLines(ship.circle_center.x, ship.circle_center.y, ship.radius*4, RED);
                    }
                    
                    // UFO
                    for (i = 0; i < 2; i++) {
                        if (ufos[i].state == UFO_ACTIVE) {
                            DrawCircle(ufos[i].pos.x, ufos[i].pos.y, 3, GREEN);
                            DrawCircleLines(ufos[i].pos.x, ufos[i].pos.y, ufos[i].radius, ORANGE);
                        }
                    }
                    // big asteroids
                    for (i = 0; i < ast_num; i++) {
                        if (asteroids[i].active) {
                            DrawCircleLines(asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].radius, ORANGE);                 // vs ship collision
                            DrawCircleLines(asteroids[i].pos.x, asteroids[i].pos.y, asteroids[i].radius*check1_radius, RED);      // 1st check for torps
                            // DrawCircle(asteroids[i].pos.x, asteroids[i].pos.y, 3, GREEN);                                      // center coords  
                        }
                        // DrawText(TextFormat("ast_type: %s", asteroids[0].type), 20, screen_height-30, 20, WHITE);
                    }

                    // mid asteroids
                    for (i = 0; i < mid_ast_num; i++) {
                        if (mid_asts[i].active) {
                            DrawCircleLines(mid_asts[i].pos.x, mid_asts[i].pos.y, mid_asts[i].radius, ORANGE);              
                            // DrawCircle(mid_asts[i].pos.x, mid_asts[i].pos.y, 3, GREEN); 
                            // DrawText(TextFormat("ast_type: %s", mid_asts[0].type), 20, screen_height-30, 20, WHITE);                                     
                        }
                    }

                    // lil asteroids
                    for (i = 0; i < lil_ast_num; i++) {
                        if (lil_asts[i].active) {
                            DrawCircleLines(lil_asts[i].pos.x, lil_asts[i].pos.y, lil_asts[i].radius, ORANGE);              
                            // DrawCircle(lil_asts[i].pos.x, lil_asts[i].pos.y, 3, GREEN);
                            // DrawText(TextFormat("ast_type: %s", lil_asts[0].type), 20, screen_height-30, 20, WHITE);                                       
                        }
                    }
                    
                    // torps
                    // for (i = 0; i < 4; i++) {
                    //     // if (torps[i].active)
                    //         DrawCircleLines(torps[i].pos.x, torps[i].pos.y, torps[i].radius, RED);
                    //         // DrawCircle(torps[i].pos.x, torps[i].pos.y, 1.0f, GREEN);
                    // } 
                }

                ///////////////////////////////////////////////////////

                /////////////// INFO DISPLAY ///////////////

                DrawText(TextFormat("%d", score), screen_width/10, 50, 40, RAYWHITE);
                for (i = 0; i < lives; i++) 
                    if (ships[i].active) 
                        DrawTexture(ships[i].tex, ships[i].pos.x, ships[i].pos.y, WHITE);

                ///////////////////////////////////////////

                // new game logic
                if (player_dead) {
                    DrawText("GAME OVER", spawn_point.x-200, spawn_point.y, 70.0f, RAYWHITE);
                    DrawText("Press ENTER to play again, or ESC to quit", spawn_point.x-300, spawn_point.y+100, 30.0f, RAYWHITE);
                    if (IsKeyPressed(KEY_ENTER))  {
                        if (score > high_score)
                            high_score = score;
                        
                        lives = 3;
                        score = 0;
                        level = 1;
                        first_level = true;
                        player_dead = false;
                        free(asteroids);
                        free(mid_asts);
                        free(lil_asts);
                        free(all_asts);

                        for (i = 0; i < 4; i++)
                            torps[i].active = false;
                        for (i = 0; i < 15; i++)
                            ufo_torps[i].active = false;
                        for (i = 0; i < particle_number; i++)
                            particles[i].active = false;
                        for (i = 0; i < 5; i++)
                            sticks[i].active = false;
                        
                        game_state = GAME_INIT;
                    }                         
                }
                break;
            } 
        }  
        
        EndDrawing();
    }

    // unload images
    UnloadImage(ship_img);
    UnloadImage(torp);
    UnloadImage(ufo_img);

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