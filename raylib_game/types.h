#include "raylib.h"
#include <vector>
#include <cmath>
#include <optional>
#include <algorithm>
#include <random>
#include <functional>
#include <string>
#include <iostream>

struct Enemy {
    Rectangle hitbox;
    Vector2 position;
    unsigned int health;
    float speed;
};

template <typename T>
T Clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Color lightenColor(Color color, float factor) {
    color.r = (unsigned char)(color.r + factor);
    color.g = (unsigned char)(color.g + factor);
    color.b = (unsigned char)(color.b + factor);
    return color;
}

Color darkenColor(Color color, float factor) {
    color.r = (unsigned char)Clamp((int)(color.r - 255 * factor), 0, 255);
    color.g = (unsigned char)Clamp((int)(color.g - 255 * factor), 0, 255);
    color.b = (unsigned char)Clamp((int)(color.b - 255 * factor), 0, 255);
    return color;
}

class Button {
private:
    Rectangle button =  { 0, 0, 0, 0 };
	std::string text = "";
    Color color = WHITE;
    Color preserve = WHITE;
public:
    Button(float x, float y, float width, float height, std::string text, Color color) {
        this->button = { x, y, width, height };
        this->text = text;
        this->color = color;
        this->preserve = color;
    }

    void updateButton(std::function<void()> func) {
        Vector2 mousePos = GetMousePosition();
        bool isHovered = CheckCollisionPointRec(mousePos, this->button);
        bool isPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if (isHovered) {
            this->color = lightenColor(this->preserve, 50);
            if (isPressed) {
                func();
            }
        }
        else {
            this->color = this->preserve;
        }
    }

    void drawButton() const {
        Color outlineColor = darkenColor(this->color, 20);
        DrawRectangle(this->button.x, this->button.y, this->button.width, this->button.height, this->color); // main body
        DrawRectangleLines(this->button.x, this->button.y, this->button.width, this->button.height, outlineColor); // outline
        int textWidth = MeasureText(this->text.c_str(), 20);
        DrawText(this->text.c_str(), this->button.x + (this->button.width - textWidth) / 2, this->button.y + (this->button.height - 20) / 2, 20, WHITE); // text
    }
};

struct Player {
    Rectangle hitbox = { 30, 30, 40, 40 };
    Vector2 position;
    unsigned char hearts = 100;
    float speed;
};

struct Bullet {
    Vector2 position;
    Vector2 direction;
    float speed;
    float radius;
};