#include "raylib.h"
#include <vector>
#include <cmath>

using namespace std;

bool checkCollisionPW(const Rectangle& player, const vector<Rectangle>& walls) {
    for (const auto& wall : walls) {
        if (CheckCollisionRecs(player, wall)) {
            return true;
        }
    }
    return false;
}

struct Enemy {
    Rectangle hitbox = { 30, 30, 40, 40 };
	Vector2 position;
    unsigned int health;
    unsigned int width;
	unsigned int height;
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

bool checkCollisionBullet(const Bullet& bullet, const vector<Rectangle>& walls) {
    for (const auto& wall : walls) {
        if (CheckCollisionCircleRec(bullet.position, bullet.radius, wall)) {
            return true;
        }
    }
    return false;
}


int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1030;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    Player player;
    Vector2 playerCenter = { player.hitbox.x + player.hitbox.width / 2.0f, player.hitbox.y + player.hitbox.height / 2.0f };
    Rectangle map = { 0, 0, 5000, 4000 };
    vector<Bullet> ammos = {};
    float shootCooldown = 0.5f;
    float shootTimer = 0.0f;

    bool debug = false;

    vector<Rectangle> walls = {
        { 0, 0, 20, 5000 },
        { 0, 0, 5000, 20 },
        { 0, 3980, 5000, 20 },
        { 4980, 0, 20, 4000 },
        { 500, 200, 300, 20 },
        { 800, 600, 20, 400 },
        { 1000, 1000, 400, 20 },
        { 1500, 1500, 20, 400 },
        { 2000, 2000, 400, 20 }
    };

    Camera2D camera = { 0 };
    camera.target = { player.hitbox.x + player.hitbox.width / 2.0f, player.hitbox.y + player.hitbox.height / 2.0f };
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    unsigned char dashPoints = 200;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Rectangle nextPosition = player.hitbox;
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
            direction = { direction.x / length, direction.y / length };

            Bullet bullet = {
                playerCenter,         // Start position
                direction,            // Direction vector
                15.0f,                 // Speed
                10.0f                 // Radius
            };
            ammos.push_back(bullet);

            shootTimer = shootCooldown;
        }
        if (IsKeyDown(KEY_LEFT_SHIFT))
        {
            if (IsKeyDown(KEY_D) && dashPoints > 20) movement.x += 10;
            if (IsKeyDown(KEY_A) && dashPoints > 20) movement.x -= 10;
            if (IsKeyDown(KEY_W) && dashPoints > 20) movement.y -= 10;
            if (IsKeyDown(KEY_S) && dashPoints > 20) movement.y += 10;

            dashPoints = max(dashPoints - 5, 0);
        }

        nextPosition.x += movement.x;
        nextPosition.y += movement.y;

        if (!checkCollisionPW(nextPosition, walls)) {
            player.hitbox = nextPosition;
            playerCenter = { player.hitbox.x + player.hitbox.width / 2.0f, player.hitbox.y + player.hitbox.height / 2.0f };
        }

        dashPoints = min(dashPoints + 1, 200);
        camera.target = { player.hitbox.x + player.hitbox.width / 2.0f, player.hitbox.y + player.hitbox.height / 2.0f };

        float zoomChange = GetMouseWheelMove() * 0.05f;
        if (zoomChange != 0.0f) {
            camera.zoom += zoomChange;
            if (camera.zoom > 3.0f) camera.zoom = 3.0f;
            else if (camera.zoom < 0.1f) camera.zoom = 0.1f;
        }

        if (IsKeyPressed(KEY_R))
        {
            camera.zoom = 1.0f;
            camera.rotation = 0.0f;
        }

        if (IsKeyPressed(KEY_F))
        {
            debug = !debug;
        }
        shootTimer -= GetFrameTime();

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


        BeginDrawing();

        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        DrawRectangleRec(map, DARKGRAY);

        for (const auto& wall : walls)
        {
            DrawRectangleRec(wall, LIGHTGRAY);
        }

        DrawRectangleRec(player.hitbox, RED);

        for (const auto& bullet : ammos)
        {
            DrawCircleV(bullet.position, bullet.radius, DARKBLUE);
        }

        if (debug) {
            Vector2 mousePosition = GetMousePosition();
            Vector2 worldMousePosition = {
                (mousePosition.x - camera.offset.x) / camera.zoom + camera.target.x,
                (mousePosition.y - camera.offset.y) / camera.zoom + camera.target.y
            };
            Vector2 direction = { worldMousePosition.x - playerCenter.x, worldMousePosition.y - playerCenter.y };
            DrawLineV(playerCenter, worldMousePosition, RED);
            DrawLine((int)camera.target.x, -screenHeight * 10, (int)camera.target.x, screenHeight * 10, GREEN);
            DrawLine(-screenWidth * 10, (int)camera.target.y, screenWidth * 10, (int)camera.target.y, GREEN);
        }
        EndMode2D();

        DrawRectangle(10, 10, 250, 40, Fade(SKYBLUE, 0.5f));

        DrawRectangle(10, 10, (int)(250 * (dashPoints / 200.0f)), 40, DARKBLUE);

        DrawRectangleLines(10, 10, 250, 40, BLUE);

        DrawText(TextFormat("Dash Points: %i", dashPoints), 20, 20, 25, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
