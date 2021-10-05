#ifndef BALL_H
#define BALL_H

#include "raylib.h"
#include "GameUtils.h"

struct Ball {
    void init();
    void update(float dt);
    void draw();
    void unload();

    bool alive;
    Vector2 lastpos;
    Circle circle;
    const float friction{0.5f};
    const float bounciness{0.75f};
};

#endif
