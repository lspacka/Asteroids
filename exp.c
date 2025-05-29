// explosion test

#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "types.h"
#include "funcs.h"

// typedef struct Particle {
//     Vector2 pos;
//     Timer timer;
//     bool dir;
//     bool active;
// } Particle;

// int main()
// {
//     SetTraceLogLevel(LOG_WARNING);
//     srand(time(NULL));
//     InitWindow(600, 600, "EXPLOSION TEST");   
//     SetTargetFPS(60);

//     bool is_pressed = false;
//     bool right = true;
//     int xdir = rand() % 2;
//     float xspeed = 0.5f;
//     float yspeed = 0.5f;
//     float ptime = 0.0f;
//     Particle particle = { 0 };

//     particle.pos.x = 300;
//     particle.pos.y = 300;
//     particle.active = false;

//     right = rand() % 1;
//     ptime = (float)rand() / RAND_MAX;

//     while(!WindowShouldClose()) {
//         BeginDrawing();
//         ClearBackground(BLACK);

//         if (IsKeyPressed(KEY_SPACE)) {
//             is_pressed = !is_pressed;
//             startTimer(&particle.timer, ptime);
//         }

//         if (is_pressed) {
//             // right = rand() % 2;
//             // ptime = rand() % 2;
//             if (right)
//                 particle.pos.x += xdir * xspeed;
//             else
//                 particle.pos.x -= xdir * xspeed;

//             particle.pos.y -= yspeed;
//             particle.active = true;
            
//             if (particle.active)
//                 DrawPixel(particle.pos.x, particle.pos.y, WHITE);

//             if (TimerDone(particle.timer))
//                 particle.active = false;
//         }
            
//         EndDrawing();
//     }
//     CloseWindow();

//     return 0;
// }

#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct Particle {
    Vector2 pos;
    // float timer; 
    float angle; 
    bool active;
    Timer timer;
} Particle;

int main() {
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));
    InitWindow(600, 600, "EXPLOSION TEST");
    SetTargetFPS(60);

    Particle particle = {0};
    Particle particles[10] = {0};
    particle.pos.x = 300;
    particle.pos.y = 300;
    particle.active = false;

    float speed = 0.5f;    // Overall speed of particle (pixels per frame)
    float duration = 2.0f; // Particle disappears after 2 seconds
    bool is_pressed = false;

    while (!WindowShouldClose()) {
        // Input handling
        if (IsKeyPressed(KEY_SPACE)) {
            is_pressed = !is_pressed;
            if (is_pressed) {
                particle.pos.x = 300; // Reset position
                particle.pos.y = 300;
                // Random angle between -45° and 45° (converted to radians)
                particle.angle = (float)(rand() % 361) * DEG2RAD;
                // particle.timer = duration; // Start timer
                startTimer(&particle.timer, duration);
                particle.active = true;
            } else {
                particle.active = false; // Stop particle if toggled off
            }
        }

        // Update particle
        if (is_pressed && particle.active) {
            // Move particle based on angle
            particle.pos.x += speed * cosf(particle.angle); // Horizontal component
            particle.pos.y -= speed * sinf(particle.angle); // Vertical component (upward)

            if (TimerDone(particle.timer))
                particle.active = false;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (particle.active) {
            DrawPixel((int)particle.pos.x, (int)particle.pos.y, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}