#pragma once
#include "raylib.h"

struct Enemy {
    Rectangle hitbox;
    Vector2 position;
    unsigned int health;
    float speed;
};

struct Player {
    Rectangle hitbox = { 30, 30, 40, 40 };
    Vector2 position;
    float speed;
};

struct Bullet {
    Vector2 position;
    Vector2 direction;
    float speed;
    float radius;
};