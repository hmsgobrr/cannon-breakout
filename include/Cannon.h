#ifndef CANNON_H
#define CANNON_H

#include "raylib.h"
#include "Utils.h"

struct Cannon {
    void init();
    void update(float dt);
    void draw();
    void unload();

    void fire(Circle& ball);

    Vector2 pos;
    float velX;
    float rot;
    const float speed{400.0f};
    const float force{1000.0f};
    bool loaded;
};


#endif
