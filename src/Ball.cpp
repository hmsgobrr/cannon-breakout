#include "Ball.h"
#include "Game.h"
#include "Utils.h"
#include "raymath.h"

void Ball::init() {
    circle = { Game::sWidth/2.0f, 500, 0, 500, 15.0f };
    alive = true;
}

void Ball::update(float dt) {
    lastpos = { circle.x, circle.y };

    if ((circle.x + circle.vx*dt) < 10 || (circle.x + circle.vx*dt) > Game::sWidth-10)
        circle.vx = circle.vx*-1*bounciness;
    if ((circle.y + circle.vy*dt) < 10 || (circle.y + circle.vy*dt) > Game::sHeight-10){
        alive = false;
        Game::pause = false;
        Game::shake = 0.5f;
        PlaySound(Game::deathSfx);
    }

    for (int i = 0; i < Game::blockNums; i++) {
        if (CheckCollisionCircleRec({(circle.x+circle.vx*dt),circle.y},circle.radius,Game::blocks[i].rec) && Game::blocks[i].active) {
            circle.vx = circle.vx*-1*bounciness;
            Game::blocks[i].active = false;
            Game::shake = 0.25f;
            PlaySound(Game::hitSfx);
        }
        if (CheckCollisionCircleRec({circle.x,(circle.y+circle.vy*dt)},circle.radius,Game::blocks[i].rec) && Game::blocks[i].active) {
            circle.vy = circle.vy*-1*bounciness;
            Game::blocks[i].active = false;
            Game::shake = 0.25f;
            PlaySound(Game::hitSfx);
        }
    }

    if (Game::cannon.loaded && (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
        Game::cannon.fire(circle);
    }

    if (circle.vx > 0) circle.vx -= friction;
    if (circle.vx < 0) circle.vx += friction;
    if (circle.vy > 0) circle.vy -= friction;
    if (circle.vy < 0) circle.vy += friction;

    circle.vx = Clamp(circle.vx, -1000, 1000);
    circle.vy = Clamp(circle.vy, -1000, 1000);

    circle.x += circle.vx * dt;
    circle.y += circle.vy * dt;

    if (Game::cannon.loaded) {
        circle.x = Game::cannon.pos.x;
        circle.y = Game::cannon.pos.y;
    }
}

void Ball::draw() {
    DrawCircleV(lastpos, 7.5f, CAROLINABLUE);
    DrawCircle(circle.x, circle.y, 15, CAROLINABLUE);
}

void Ball::unload() {
    // TODO: unload tings
}
