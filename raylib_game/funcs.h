#pragma once
#include "raylib.h"
#include "types.h"
#include <vector>
#include <cmath>
#include <optional>

using namespace std;

bool checkCollisionPW(const Rectangle& player, const vector<Rectangle>& walls) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs(player, wall)) {
            return true;
        }
    }
    return false;
}

bool checkCollisionBullet(const Bullet& bullet, const vector<Rectangle>& walls) {
    for (const auto& wall : walls) {
        if (CheckCollisionCircleRec(bullet.position, bullet.radius, wall)) {
            return true;
        }
    }
    return false;
}

template <typename T>
T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void keyHandle(vector<Rectangle>& walls, vector<Bullet>& ammos, Player& player, Camera2D& camera, unsigned char& dashPoints, float& shootTimer, float shootCooldown, Vector2& playerCenter) {
    Vector2 movement = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_D)) movement.x += 10;
    if (IsKeyDown(KEY_A)) movement.x -= 10;
    if (IsKeyDown(KEY_W)) movement.y -= 10;
    if (IsKeyDown(KEY_S)) movement.y += 10;

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && shootTimer <= 0.0f) {
        Vector2 mousePosition = GetMousePosition();
        Vector2 worldMousePosition = {
            (mousePosition.x - camera.offset.x) / camera.zoom + camera.target.x,
            (mousePosition.y - camera.offset.y) / camera.zoom + camera.target.y
        };
        Vector2 direction = { worldMousePosition.x - playerCenter.x, worldMousePosition.y - playerCenter.y };

        float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
        direction.x /= length;
        direction.y /= length;

        Bullet bullet = { playerCenter, direction, 15.0f, 10.0f };
        ammos.push_back(bullet);

        shootTimer = shootCooldown;
    }

    if (IsKeyDown(KEY_LEFT_SHIFT) && dashPoints > 20) {
        movement.x *= 2;
        movement.y *= 2;
        dashPoints = max(dashPoints - 5, 0);
    }

    Rectangle nextPosition = player.hitbox;
    nextPosition.x += movement.x;
    nextPosition.y += movement.y;

    if (!checkCollisionPW(nextPosition, walls)) {
        player.hitbox = nextPosition;
        playerCenter = { player.hitbox.x + player.hitbox.width / 2.0f, player.hitbox.y + player.hitbox.height / 2.0f };
    }

    dashPoints = min(dashPoints + 1, 200);
    camera.target = { player.hitbox.x + player.hitbox.width / 2.0f, player.hitbox.y + player.hitbox.height / 2.0f };
    float zoomChange = GetMouseWheelMove() * 0.05f;
    camera.zoom = clamp(camera.zoom + zoomChange, 0.1f, 3.0f);

    if (IsKeyPressed(KEY_R)) {
        camera.zoom = 1.0f;
        camera.rotation = 0.0f;
    }
}


void updateBullets(vector<Bullet>& ammos, vector<Rectangle>& walls, const optional<Enemy>& enemy) {
    for (auto it = ammos.begin(); it != ammos.end(); ) {
        Bullet& bullet = *it;
        bullet.position.x += bullet.direction.x * bullet.speed;
        bullet.position.y += bullet.direction.y * bullet.speed;

        if (checkCollisionBullet(bullet, walls)) {
            it = ammos.erase(it);
        }
        else {
            ++it;
        }
    }
}

void drawDebug(const Player& player, const Camera2D& camera, const Vector2& playerCenter, int screenWidth, int screenHeight) {
    Vector2 mousePosition = GetMousePosition();
    Vector2 worldMousePosition = {
        (mousePosition.x - camera.offset.x) / camera.zoom + camera.target.x,
        (mousePosition.y - camera.offset.y) / camera.zoom + camera.target.y
    };
    DrawLineV(playerCenter, worldMousePosition, RED);
    DrawLine((int)camera.target.x, -screenHeight * 10, (int)camera.target.x, screenHeight * 10, GREEN);
    DrawLine(-screenWidth * 10, (int)camera.target.y, screenWidth * 10, (int)camera.target.y, GREEN);
}
