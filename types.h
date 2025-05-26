#ifndef TYPES_H
#define TYPES_H

typedef enum GameState {
    GAME_INIT,
    GAME_TRANSIT,
    GAME_ACTIVE,
    GAME_NEW
} GameState;

typedef enum UFOState {
    UFO_DEAD,
    UFO_WAITING,
    UFO_SPAWNING,
    UFO_ACTIVE
} UFOState;

typedef enum UFOShootState {
    SHOOT_COOLDOWN,
    SHOOT_TRANSIT,
    SHOOT_FIRE
} UFOShootState;

typedef struct Timer {
    double startTime;
    double lifeTime;
} Timer;

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
    // ShipState state;
    float rotation;     //
    float lowSpeed;
    float scale;        //
    float radius;
    int lives;
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
    Vector2 center;
    Vector2 shootDir;
    Rectangle rect;
    Rectangle bounds;
    Texture2D tex;
    bool right;         // 0==left->rigth,  1==right->left
    bool deviate;
    bool up;
    bool onscreen;
    bool shoot;        //  
    float radius;
    Timer respawnTimer;
    Timer movementTimer;
    Timer shootingTimer;
    Timer cooldownTimer;
    Timer transitTimer;
    Timer torpTimer;
    UFOState state;
    UFOShootState shoot_state;
    // int state;
    char* name;         // just for testing
    Color* pix;         //
    float scale;        //
    bool active;        //
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
    Timer timer;
    float radius;
    bool active;
    float shoot_angle;
} Torp;

#endif