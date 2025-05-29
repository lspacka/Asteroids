#ifndef FUNCS_H
#define FUNCS_H

#include <string.h>
#include <math.h>
#include "types.h"
#include "raylib.h"

int RandPos(int a, int b);
float GetRandomFloat(float min, float max);
double getElapsed(Timer timer);
void startTimer(Timer* timer, double lifetime);
void AstBlast(Asteroid ast, Asteroid* asts, int* index, Texture2D sprites[]);
void ShipSpawn(Ship* ship, int lives, Asteroid* asteroids, int astnum, Timer timer, Vector2 spawnpoint, bool* playerstate);
bool TimerDone(Timer timer);

int RandPos(int a, int b)
{
    return a + rand() % (b - a+1);
}


float GetRandomFloat(float min, float max) 
{
    return min + (GetRandomValue(0, 1000) / 1000.0f) * (max - min);
}

void ShipSpawn(
                Ship* ship, 
                int lives, 
                Asteroid* asteroids, 
                int astnum, 
                Timer timer, 
                Vector2 spawnpoint, 
                bool* playerstate
              )
{
    // if (!ship->active && lives > 0) {
    //     if (TimerDone(timer)) {
    //         // reset ship state
    //         ship->vel.x = 0;
    //         ship->vel.y = 0;
    //         ship->pos = spawnpoint;
    //         // Check if spawn area is clear
    //         bool spawn_area_clear = true;
    //         float spawn_radius = ship->radius*16;
    //         Vector2 spawn_point = spawnpoint;
    //         for (int j = 0; j < astnum; j++) {  // add safe_radius attr to Ship
    //             if (asteroids[j].active && CheckCollisionCircles(spawnpoint, ship->radius*16, asteroids[j].pos, asteroids[j].radius)) {
    //                 spawn_area_clear = false;
    //                 break;
    //             }
    //         }

    //         if (spawn_area_clear) {
    //             ship->active = true;
    //         } else {
    //             // Restart timer to try again next frame
    //             startTimer(&timer, 0.5f); // Short delay to avoid excessive checks
    //         }
    //     }
    // } else if (lives <= 0 && !ship->active) {
    //     *playerstate = true; 
    // }

    if (!ship->active && lives > 0) {
        if (TimerDone(timer)) {
            Vector2 spawn_point = spawnpoint;
            float safe_width = spawnpoint.x / 2;  // ~1/4 screen width, like Asteroids
            float safe_height = spawnpoint.y / 2; // ~1/4 screen height
            bool spawn_area_clear = true;

            // Check asteroid centers in rectangular safe zone
            for (int j = 0; j < astnum; j++) {
                if (!asteroids[j].active) continue;
                float dx = fabs(asteroids[j].pos.x - spawn_point.x);
                float dy = fabs(asteroids[j].pos.y - spawn_point.y);
                if (dx < safe_width && dy < safe_height) {
                    spawn_area_clear = false;
                    break;
                }
            }

            // Minimal velocity check for asteroids just outside safe zone
            if (spawn_area_clear) {
                for (int j = 0; j < astnum; j++) {
                    if (!asteroids[j].active) continue;
                    float dx = asteroids[j].pos.x - spawn_point.x;
                    float dy = asteroids[j].pos.y - spawn_point.y;
                    float distance = sqrtf(dx * dx + dy * dy);
                    if (distance < safe_width * 1.5) { // Check slightly larger area
                        float dot_product = dx * asteroids[j].speed.x + dy * asteroids[j].speed.y;
                        if (dot_product < 0) { // Moving toward spawn point
                            spawn_area_clear = false;
                            break;
                        }
                    }
                }
            }

            if (spawn_area_clear) {
                ship->pos.x = spawnpoint.x;
                ship->pos.y = spawnpoint.y;
                ship->active = true;
                // Optional: Brief invulnerability
                // ship.invulnerable = true;
                // startTimer(&ship_invulnerability_timer, 1.0f);
            } else {
                startTimer(&timer, 0.1f); // Retry every 1/60s, like Asteroids
            }

            // Debug visualization: Draw safe zone rectangle
            // Rectangle safe_zone = {
            //     spawn_point.x - safe_width,
            //     spawn_point.y - safe_height,
            //     safe_width * 2,
            //     safe_height * 2
            // };
            // DrawRectangleLinesEx(safe_zone, 2.0f, spawn_area_clear ? GREEN : RED);
        }
    } else if (lives <= 0 && !ship->active) {
        *playerstate = true;
    }
}

// void ShipSpawn2(
//                 Ship* ship, 
//                 int lives, 
//                 UFO* ufos, 
//                 int ufonum, 
//                 Timer timer, 
//                 Vector2 spawnpoint, 
//                 GameState* gamestate
//               )
// {
//     if (!ship->active && lives > 0) {
//         if (TimerDone(timer)) {
//             // reset ship state
//             ship->vel.x = 0;
//             ship->vel.y = 0;
//             ship->pos = spawnpoint;
//             // Check if spawn area is clear
//             bool spawn_area_clear = true;
//             for (int j = 0; j < ufonum; j++) { 
//                 if (ufos[j].active && CheckCollisionCircles(spawnpoint, ship->radius*6, ufos[j].pos, ufos[j].radius)) {
//                     spawn_area_clear = false;
//                     break;
//                 }
//             }

//             if (spawn_area_clear) {
//                 ship->active = true;
//             } else {
//                 // Restart timer to try again next frame
//                 startTimer(&timer, 0.5f); // Short delay to avoid excessive checks
//             }
//         }
//     } else if (lives <= 0 && !ship->active) {
//         *gamestate = GAME_OVER; 
//     }
// }

void AstBlast(Asteroid ast, Asteroid* asts, int* index, Texture2D sprites[])
{
    int i, direction, prev_dir = -1, spr_ind;
    float new_speed;

    for (i = 0; i < 2; i++) {
        // do {
        //     direction = rand() % 4;
        // } while (direction == ast.dir || (i == 1 && direction == prev_dir));

        // prev_dir = direction;
        direction = rand() % 4;

        spr_ind = GetRandomValue(0, 15);
        new_speed = GetRandomFloat(.97f, 3.7f);
        asts[*index].active = true;
        asts[*index].tex = sprites[spr_ind];
        // asts[*index].tex = ast.tex;

        if (strcmp(ast.type, "big") == 0) {
            asts[*index].type = "mid";
            asts[*index].tex.width *= 0.7;
            asts[*index].tex.height *= 0.7;
        } else if (strcmp(ast.type, "mid") == 0) {
            asts[*index].type = "lil";
            asts[*index].tex.width *= 0.5;
            asts[*index].tex.height *= 0.5;
        }

        // calc random direction?

        asts[*index].pos = ast.pos;
        asts[*index].speed.x = new_speed;
        asts[*index].speed.y = new_speed;
        asts[*index].size = (Vector2){asts[*index].tex.width, asts[*index].tex.height};
        asts[*index].center = (Vector2){asts[*index].size.x/2, asts[*index].size.y/2};
        asts[*index].rect = (Rectangle){0, 0, asts[*index].size.x, asts[*index].size.y};
        asts[*index].bounds = (Rectangle){asts[*index].pos.x, asts[*index].pos.y, asts[*index].size.x, asts[*index].size.y};
        asts[*index].dir = direction;
        asts[*index].radius = asts[*index].size.x / 2.2f;

        (*index)++;
    }
}

void UFOShoot(Ship ship, UFO* ufo) 
{
    if (strcmp(ufo->name, "sluggo") == 0) {
        // do this
    } else {
        // do this
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

#endif