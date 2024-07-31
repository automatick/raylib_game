#include "funcs.h"

void startGame(bool& gameRun) {
    gameRun = true;
}

void mainMenu(int screenWidth, int screenHeight) {
    bool gameRun = false;
    Button start(screenWidth / 2.0f - 100, screenHeight / 2.0f - 25, 200, 50, R"(Start)", GREEN);
    while (!gameRun && !WindowShouldClose()) {
        start.updateButton([&gameRun]() {
            startGame(gameRun);
            });
        BeginDrawing();
        ClearBackground(RAYWHITE);
        start.drawButton();
        EndDrawing();
    }
}
