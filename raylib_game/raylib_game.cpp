#include "funcs.h"

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
       keyHandle(walls, ammos, player, camera, dashPoints, shootTimer, shootCooldown, playerCenter);

        shootTimer -= GetFrameTime();

        if (IsKeyPressed(KEY_F)) {
            debug = !debug;
        }

        updateBullets( ammos, walls, nullopt );

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
            drawDebug(player, camera, playerCenter, screenWidth, screenHeight);
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
