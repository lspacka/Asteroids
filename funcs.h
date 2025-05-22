#ifndef FUNCS_H
#define FUNCS_H

#include "types.h"
#include "raylib.h"

int RandPos(int a, int b);
float GetRandomFloat(float min, float max);
double getElapsed(Timer timer);
void startTimer(Timer* timer, double lifetime);
void AstBlast(Asteroid ast, Asteroid* asts, int* index, Texture2D sprites[]);
void ShipSpawn(Ship* ship, int lives, Asteroid* asteroids, int astnum, Timer timer, Vector2 spawnpoint, GameState* gamestate);
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
                GameState* gamestate
              )
{
    if (!ship->active && lives > 0) {
        if (TimerDone(timer)) {
            // reset ship state
            ship->vel.x = 0;
            ship->vel.y = 0;
            ship->pos = spawnpoint;
            // Check if spawn area is clear
            bool spawn_area_clear = true;
            for (int j = 0; j < astnum; j++) { 
                if (asteroids[j].active && CheckCollisionCircles(spawnpoint, ship->radius*6, asteroids[j].pos, asteroids[j].radius)) {
                    spawn_area_clear = false;
                    break;
                }
            }

            if (spawn_area_clear) {
                ship->active = true;
            } else {
                // Restart timer to try again next frame
                startTimer(&timer, 0.5f); // Short delay to avoid excessive checks
            }
        }
    } else if (lives <= 0 && !ship->active) {
        *gamestate = GAME_OVER; 
    }
}

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