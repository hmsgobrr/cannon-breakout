#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#define GUNMETAL Color{ 18, 44, 52, 255 }
#define PRUSSIANBLUE Color{ 26, 58, 82, 255 }
#define INDIGODYE Color{ 34, 72, 112, 255 }
#define DARKCONFLOWERBLUE Color{ 42, 68, 148, 255 }
#define CAROLINABLUE Color{ 78, 165, 217, 255 }
#define MEDIUMTURQUOISE Color{ 68, 207, 203, 255 }

#include <iostream>
#define LOG(x) std::cout << x << std::endl;

#include "raylib.h"

struct Circle {
    float x;
    float y;
    float vx;
    float vy;
    float radius;
};

// void resolveBallCircleCollision(Circle& c1, Circle& c2);
// void elastic2DCollision(Circle& c1, Circle& c2, float v1, float v2, float d1, float d2, float cDir, float m1, float m2);

// bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
// bool lineRect(Vector2 pos1, Vector2 pos2, Rectangle rec);

#endif
