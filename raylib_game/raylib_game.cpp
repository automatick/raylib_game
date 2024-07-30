#include "menu.h"

using namespace std;

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1030;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");

    Player player;
    player.speed = 2.0f;
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


    vector<Enemy> enemies = {
        { { 100, 100, 40, 40 }, { 0, 0 }, 100, 5.0f },
        { { 500, 500, 40, 40 }, { 0, 0 }, 100, 5.0f }
    };

    Camera2D camera = { 0 };
    camera.target = calcPlayerCenter(player);
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    unsigned char dashPoints = 200;
    unsigned int kills = 0;

    SetTargetFPS(60);

    mainMenu();

    while (!WindowShouldClose())
    {
        keyHandle(walls, ammos, player, camera, dashPoints, shootTimer, shootCooldown, playerCenter);
        shootTimer -= GetFrameTime();
        if (IsKeyPressed(KEY_F)) {
            debug = !debug;
        }

        updateBullets(ammos, walls, enemies, kills);
        updateEnemies(enemies, player, walls, kills);
        
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
        for (const auto& enemy : enemies)
        {
            if (enemy.health > 0) {
                DrawRectangleRec(enemy.hitbox, DARKPURPLE);
            }
        }
        if (debug) {
            drawDebug(player, camera, playerCenter, screenWidth, screenHeight);
        }
        
        EndMode2D();
        
        drawUI(player, dashPoints);
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
