#include "raylib.h"
#include <stdio.h>

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

int main()
{
    printf("size of Asteroid struct: %zu\n", sizeof(Asteroid));

    return 0;
}