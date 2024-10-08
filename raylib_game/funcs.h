#include "handle_funcs.h"

void keyHandle(vector<Rectangle>& walls, vector<Bullet>& ammos, Player& player, Camera2D& camera, unsigned char& dashPoints, float& shootTimer, float shootCooldown, Vector2& playerCenter, char& dh) {
    Vector2 movement = { 0.0f, 0.0f };
    moveHandle(movement, 1.0f);

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

    dashHandle(movement, dashPoints, dh);

    Rectangle nextPosition = player.hitbox;
    nextPosition.x += movement.x;
    nextPosition.y += movement.y;

    if (!checkCollisionPW(nextPosition, walls)) {
        player.hitbox = nextPosition;
        playerCenter = calcPlayerCenter(player);
    }
    if (dh == 1) {
        dashPoints = min(dashPoints + 1, 200);
        dh = 0;
    }
    else
    {
        dh = 1;
    }
    camera.target = calcPlayerCenter(player);
    float zoomChange = GetMouseWheelMove() * 0.05f;
    camera.zoom = Clamp(camera.zoom + zoomChange, 0.1f, 3.0f);

    if (IsKeyPressed(KEY_R)) {
        camera.zoom = 1.0f;
        camera.rotation = 0.0f;
    }
}

void updateBullets(vector<Bullet>& ammos, vector<Rectangle>& walls, vector<Enemy>& enemies, unsigned int& kills, unsigned int &enemyCount) {
    for (auto it = ammos.begin(); it != ammos.end(); ) {
        Bullet& bullet = *it;
        bullet.position.x += bullet.direction.x * bullet.speed;
        bullet.position.y += bullet.direction.y * bullet.speed;

        bool hitWall = checkCollisionBullet(bullet, walls);
        bool hitEnemy = false;
        auto enemyIt = enemies.end();

        for (auto eIt = enemies.begin(); eIt != enemies.end(); ++eIt) {
            if (CheckCollisionCircleRec(bullet.position, bullet.radius, eIt->hitbox)) {
                hitEnemy = true;
                eIt->health -= 10;
                if (eIt->health <= 0) {
                    enemyIt = eIt;
                    enemyCount--;
                    kills++;
                }
                break;
            }
        }

        if (hitWall || hitEnemy) {
            if (hitEnemy && enemyIt != enemies.end()) {
                enemies.erase(enemyIt);
            }
            it = ammos.erase(it);
        }
        else {
            ++it;
        }
    }
}



vector<Rectangle> GenerateMap(int mapWidth, int mapHeight, int wallThickness) {
    vector<Rectangle> walls;
    walls.push_back(Rectangle( 0, 0, mapWidth, wallThickness ));
    walls.push_back(Rectangle( 0, 0, wallThickness, mapHeight ));
    walls.push_back(Rectangle( 0, mapHeight - wallThickness, mapWidth, wallThickness ));
    walls.push_back(Rectangle(mapWidth - wallThickness, 0, wallThickness, mapHeight ));

    int numInternalWalls = 50;
    for (int i = 0; i < numInternalWalls; ++i) {
        float x = GetRandomValue(0, mapWidth - wallThickness);
        float y = GetRandomValue(0, mapHeight - wallThickness);
        float width = GetRandomValue(wallThickness, 300);
        float height = GetRandomValue(wallThickness, 300);
        if (x + width > mapWidth) width = mapWidth - x;
        if (y + height > mapHeight) height = mapHeight - y;

        walls.push_back({ x, y, width, height });
    }

    return walls;
}

void updateEnemies(vector<Enemy>& enemies, Player& player, const vector<Rectangle>& walls, unsigned int &kills) {
    srand(time(nullptr));
    for (auto& enemy : enemies) {

        Vector2 direction = { player.hitbox.x - enemy.hitbox.x, player.hitbox.y - enemy.hitbox.y };
        float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
        direction.x /= length;
        direction.y /= length;

        float randomFactorX = (rand() % 200 - 100) / 100.0f;
        float randomFactorY = (rand() % 200 - 100) / 100.0f;
        direction.x += randomFactorX;
        direction.y += randomFactorY;
        length = sqrtf(direction.x * direction.x + direction.y * direction.y);
        direction.x /= length;
        direction.y /= length;

        Rectangle newHitbox = enemy.hitbox;
        newHitbox.x += direction.x * enemy.speed;
        newHitbox.y += direction.y * enemy.speed;

        bool collision = std::any_of(walls.begin(), walls.end(), [&](const Rectangle& wall) {
            return CheckCollisionRecs(newHitbox, wall);
            });

        if (collision) {
            bool foundAlternative = false;
            Vector2 alternativeDirections[] = {
                { -direction.y, direction.x },
                { direction.y, -direction.x }
            };

            for (const auto& altDir : alternativeDirections) {
                newHitbox.x = enemy.hitbox.x + altDir.x * enemy.speed;
                newHitbox.y = enemy.hitbox.y + altDir.y * enemy.speed;
                collision = std::any_of(walls.begin(), walls.end(), [&](const Rectangle& wall) {
                    return CheckCollisionRecs(newHitbox, wall);
                    });

                if (!collision) {
                    direction = altDir;
                    foundAlternative = true;
                    break;
                }
            }

            if (!foundAlternative) {
                direction = { 0, 0 };
            }
        }
        if (CheckCollisionRecs(player.hitbox, enemy.hitbox)) {
			direction = { 0, 0 };
            player.hearts -= 1;
        }
        enemy.hitbox.x += direction.x * enemy.speed;
        enemy.hitbox.y += direction.y * enemy.speed;
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