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
    float shootCooldown = 0.2f;
    float shootTimer = 0.0f;

    bool debug = false;

    vector<Rectangle> walls = GenerateMap(map.width, map.height, 2);

    unsigned int enemyCount = 2;
    unsigned int maxEnemies = 10;

    vector<Enemy> enemies = {
        { { 1400, 1400, 40, 40 }, { 0, 0 }, 100, 5.0f },
        { { 1400, 1400, 40, 40 }, { 0, 0 }, 100, 5.0f }
    };

    Camera2D camera = { 0 };
    camera.target = calcPlayerCenter(player);
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    unsigned char dashPoints = 200;
    unsigned int kills = 0;
    char dh = 1;
    const unsigned int MAX_ENEMIES_LIMIT = 50;
    static unsigned int lastKillCount = 0;

    SetTargetFPS(60);

    mainMenu(screenWidth, screenHeight);

    while (!WindowShouldClose())
    {
        keyHandle(walls, ammos, player, camera, dashPoints, shootTimer, shootCooldown, playerCenter, dh);
        shootTimer -= GetFrameTime();
        if (IsKeyPressed(KEY_F)) {
            debug = !debug;
        }
        if (enemyCount < maxEnemies && maxEnemies < MAX_ENEMIES_LIMIT) {
            if (kills / 10 > lastKillCount) {
                maxEnemies++;
                enemies.push_back({ { 1400, 1400, 80, 80 }, { 0, 0 }, 300, 4.0f });
                lastKillCount = kills / 10;
            }
            else {
                enemies.push_back({ { 1400, 1400, 40, 40 }, { 0, 0 }, 100, 5.0f });
            }
            enemyCount++;
        }
        updateBullets(ammos, walls, enemies, kills, enemyCount);
        updateEnemies(enemies, player, walls, kills);
        
        if (player.hearts <= 0 || player.hearts == 255) {
            goto exit_;
        }

        BeginDrawing();
        
        ClearBackground(BLACK);
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
        
        drawUI(player, dashPoints, kills);
        
        EndDrawing();
    }
    exit_:
        CloseWindow();
        return 0;
}
