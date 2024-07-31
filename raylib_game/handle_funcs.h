#include "types.h"

using namespace std;



Vector2 inline calcPlayerCenter(Player& player) {
    return { player.hitbox.x + player.hitbox.width / 2.0f, player.hitbox.y + player.hitbox.height / 2.0f };
}

void drawDash(const unsigned char& dashPoints) {
    DrawRectangle(10, 10, 250, 40, Fade(SKYBLUE, 0.5f));
    DrawRectangle(10, 10, (int)(250 * (dashPoints / 200.0f)), 40, DARKBLUE);
    DrawRectangleLines(10, 10, 250, 40, BLUE);
    DrawText(TextFormat("Dash Points: %i", dashPoints), 20, 20, 25, WHITE);
}

void drawHearts (const Player& player) {
    DrawRectangle(10, 80, 250, 40, Fade(GREEN, 0.5f));
    DrawRectangle(10, 80, (int)(250 * (player.hearts / 100.0f)), 40, DARKGREEN);
    DrawRectangleLines(10, 80, 250, 40, GREEN);
    DrawText(TextFormat("Hearts: %i", player.hearts), 20, 90, 25, WHITE);

}

void printKills(const unsigned int& kills) {
	DrawText(TextFormat("Kills: %i", kills), 20, 120, 25, WHITE);
}

void inline drawUI(const Player& player, const unsigned char &dashPoints, const unsigned int& kills) {
    drawDash(dashPoints);
	drawHearts(player);
    printKills(kills);
}

void inline moveHandle(Vector2 &movement, const unsigned int speedCoeff) {
    if (IsKeyDown(KEY_D)) movement.x += (5 * speedCoeff);
    if (IsKeyDown(KEY_A)) movement.x -= (5 * speedCoeff);
    if (IsKeyDown(KEY_W)) movement.y -= (5 * speedCoeff);
    if (IsKeyDown(KEY_S)) movement.y += (5 * speedCoeff);
}

void inline dashHandle(Vector2& movement, unsigned char &dashPoints, char &dh) {
    if (IsKeyDown(KEY_LEFT_SHIFT) && dashPoints > 0) {
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