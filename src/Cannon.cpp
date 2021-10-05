#include "Cannon.h"
#include "Game.h"
#include <math.h>

void Cannon::init() {
    pos = { Game::sWidth/2.0f, Game::sHeight - 60.0f };
    rot = 0;
    velX = 0;
    loaded = false;
}

void Cannon::update(float dt) {
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) velX = 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) velX = -1;

    if (CheckCollisionCircles(pos,30, {Game::ball.circle.x, Game::ball.circle.y},Game::ball.circle.radius))
        loaded = true;

    pos.x += velX*speed*dt;
    velX = 0;

    if (pos.x < 0) pos.x = Game::sWidth;
    if (pos.x > Game::sWidth) pos.x = 0;

    rot = atan2f(GetMouseY() - pos.y, GetMouseX() - pos.x)*RAD2DEG;
}

void Cannon::draw() {
    DrawRectanglePro({ pos.x, pos.y, 50, 30 }, { 0, 15 }, rot, DARKCONFLOWERBLUE);
    DrawCircleV(pos, 25, DARKCONFLOWERBLUE);
    if (loaded) DrawCircleV(pos, 15, CAROLINABLUE);
}

void Cannon::unload() {
    // TODO: unload tings
}

void Cannon::fire(Circle& ball) {
    ball.x = pos.x + cosf(rot*DEG2RAD)*50;
    ball.y = pos.y + sinf(rot*DEG2RAD)*50;
    ball.vx = force*cosf(rot*DEG2RAD);
    ball.vy = force*sinf(rot*DEG2RAD);
    loaded = false;
}
