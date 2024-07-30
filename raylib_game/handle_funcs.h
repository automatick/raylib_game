#include "raylib.h"
#include "types.h"
#include <vector>
#include <cmath>
#include <optional>
#include <algorithm>
#include <random>

using namespace std;


Vector2 inline calcPlayerCenter(Player& player) {
    return { player.hitbox.x + player.hitbox.width / 2.0f, player.hitbox.y + player.hitbox.height / 2.0f };
}

void drawDash(unsigned char dashPoints) {
    DrawRectangle(10, 10, 250, 40, Fade(SKYBLUE, 0.5f));
    DrawRectangle(10, 10, (int)(250 * (dashPoints / 200.0f)), 40, DARKBLUE);
    DrawRectangleLines(10, 10, 250, 40, BLUE);
    DrawText(TextFormat("Dash Points: %i", dashPoints), 20, 20, 25, WHITE);
}

void drawHearts (Player& player) {
    DrawRectangle(10, 80, 250, 40, Fade(GREEN, 0.5f));
    DrawRectangle(10, 80, (int)(250 * (player.hearts / 100.0f)), 40, DARKGREEN);
    DrawRectangleLines(10, 80, 250, 40, GREEN);
    DrawText(TextFormat("Hearts: %i", player.hearts), 20, 90, 25, WHITE);

}

void inline drawUI(Player& player, unsigned char dashPoints) {
    drawDash(dashPoints);
	drawHearts(player);
}

void inline moveHandle(Vector2 &movement, const unsigned int speedCoeff) {
    if (IsKeyDown(KEY_D)) movement.x += (5 * speedCoeff);
    if (IsKeyDown(KEY_A)) movement.x -= (5 * speedCoeff);
    if (IsKeyDown(KEY_W)) movement.y -= (5 * speedCoeff);
    if (IsKeyDown(KEY_S)) movement.y += (5 * speedCoeff);
}

void inline dashHandle(Vector2& movement, unsigned char &dashPoints) {
    if (IsKeyDown(KEY_LEFT_SHIFT) && dashPoints > 20) {
        movement.x *= 2;
        movement.y *= 2;
        dashPoints = max(dashPoints - 5, 0);
    }
}

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
concept Number = std::is_arithmetic_v<T>;

template <typename Number>
Number Clamp(Number value, Number min, Number max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}