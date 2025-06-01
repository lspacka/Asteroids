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
void ShipSpawn(Ship* ship, int lives, Asteroid* asteroids, int astnum, Timer timer, Vector2 spawnpoint, bool* playerdead);
void pof(Vector2 pos, float radius, Particle* particle, const int particlenum);
void desint(Ship ship, Stick* sticks, float* spawntime);
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
                bool* playerdead
                // UFO ufo
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
            float spawn_radius = ship->radius*16;
            Vector2 spawn_point = spawnpoint;
            for (int j = 0; j < astnum; j++) {  // add safe_radius attr to Ship
                if (asteroids[j].active && 
                    (CheckCollisionCircles(spawnpoint, ship->radius*16, asteroids[j].pos, asteroids[j].radius) || 
                    CheckCollisionCircles(spawnpoint, ship->radius*14, asteroids[j].pos, asteroids[j].radius)  || 
                    CheckCollisionCircles(spawnpoint, ship->radius*12, asteroids[j].pos, asteroids[j].radius)  || 
                    CheckCollisionCircles(spawnpoint, ship->radius*10, asteroids[j].pos, asteroids[j].radius)  ||
                    CheckCollisionCircles(spawnpoint, ship->radius*8, asteroids[j].pos, asteroids[j].radius)   ||
                    CheckCollisionCircles(spawnpoint, ship->radius*6, asteroids[j].pos, asteroids[j].radius)   ||
                    CheckCollisionCircles(spawnpoint, ship->radius*4, asteroids[j].pos, asteroids[j].radius)   ||
                    CheckCollisionCircles(spawnpoint, ship->radius*1.7, asteroids[j].pos, asteroids[j].radius) ||
                    CheckCollisionCircles(spawnpoint, ship->radius, asteroids[j].pos, asteroids[j].radius)     || 
                    CheckCollisionPointCircle(spawnpoint, asteroids[j].center, asteroids[j].radius))) {
                    spawn_area_clear = false;
                    break;
                }
            }

            if (spawn_area_clear) {
                ship->active = true;
            } else {
                // Restart timer to try again next frame
                startTimer(&timer, 0.05f); // Short delay to avoid excessive checks
            }
        }
    } else if (lives <= 0 && !ship->active) {
        *playerdead = true; 
    }
}

void AstBlast(Asteroid ast, Asteroid* asts, int* index, Texture2D sprites[])
{
    int i, direction, prev_dir = -1, spr_ind;
    float new_speed;

    for (i = 0; i < 2; i++) {
        direction = rand() % 4;
        spr_ind = GetRandomValue(0, 15);
        new_speed = GetRandomFloat(.97f, 3.7f);
        asts[*index].active = true;
        asts[*index].tex = sprites[spr_ind];

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

void pof(Vector2 pos, float radius, Particle* particles, const int particlenum) 
{
    int i;
    float speed;
    float duration;
    float max = radius / 19.0f;
    int steps;

    for (i = 0; i < particlenum; i++) {
        particles[i].pos.x = pos.x;
        particles[i].pos.y = pos.y;
        particles[i].active = false;
    }

    for (i = 0; i < particlenum; i++) {
        steps = (int)((max - 0.3f) / 0.1f) + 1;
        duration = (float)rand() / RAND_MAX; 
        particles[i].speed = 0.3f + (float)(rand() % steps) * 0.1f;   // the bigger the faster 
        particles[i].angle = (float)(rand() % 361) * DEG2RAD; 
        startTimer(&particles[i].timer, duration);
        particles[i].active = true;
    }
}

void desint(Ship ship, Stick* sticks, float* spawntime) 
{
    int i;
    float rotation;
    float duration = 0.7f;

    for (i = 0; i < 5; i++) {
        rotation = rand() % 361;     // (float)rand() / RAND_MAX - 0.5f;
        // duration = (float)rand() / RAND_MAX;
        sticks[i].center = (Vector2){sticks[i].size.x/2, sticks[i].size.y/2};
        sticks[i].pos.x = (ship.pos.x - 5) + rand() % 11;
        sticks[i].pos.y = (ship.pos.y - 5) + rand() % 11;
        sticks[i].rotation = rotation;
        sticks[i].bounds = (Rectangle){sticks[i].pos.x, sticks[i].pos.y, sticks[i].size.x, sticks[i].size.y};
        startTimer(&sticks[i].timer, duration);
        duration += 0.7f;
        sticks[i].active = true;
    }
    *spawntime = duration;   // time for the ship to spawn
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