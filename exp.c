// explosion test

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "raylib.h"
#include "types.h"
#include "funcs.h"

typedef struct Particle {
    Vector2 pos;
    Timer timer;
    float angle; 
    float speed;
    bool active;
} Particle;

int main() {
    SetTraceLogLevel(LOG_WARNING);
    srand(time(NULL));
    InitWindow(600, 600, "EXPLOSION TEST");
    SetTargetFPS(60);

    int i;
    int p_quant = 19;
    Particle* particles = (Particle*)malloc(p_quant * sizeof(Particle));
    
    for (i = 0; i < p_quant; i++) {
        particles[i].pos.x = 300;
        particles[i].pos.y = 300;
        particles[i].active = false;
    }

    float speed;
    float duration;
    bool is_pressed = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            is_pressed = !is_pressed;

            for (i = 0; i < p_quant; i++) {
                if (is_pressed) {
                    duration = (float)rand() / RAND_MAX;
                    particles[i].speed = 0.3f + (float)(rand() % 8) / 10.0f;
                    particles[i].pos.x = 300;   // Reset position
                    particles[i].pos.y = 300;
                    particles[i].angle = (float)(rand() % 361) * DEG2RAD;      // random angle. 120-61
                    startTimer(&particles[i].timer, duration);
                    particles[i].active = true;
                } else {
                    particles[i].active = false;
                }
            }
        }

        // Update particles
        for (i = 0; i < p_quant; i++) {
            if (is_pressed && particles[i].active) {
                particles[i].pos.x += particles[i].speed * cosf(particles[i].angle);
                particles[i].pos.y -= particles[i].speed * sinf(particles[i].angle);

                if (TimerDone(particles[i].timer))
                    particles[i].active = false;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        for (i = 0; i < p_quant; i++)
            if (particles[i].active) {
                DrawPixel((int)particles[i].pos.x, (int)particles[i].pos.y, WHITE);
            }

        EndDrawing();
    }

    free(particles);
    CloseWindow();

    return 0;
}