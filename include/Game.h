#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Ball.h"
#include "Cannon.h"
#include "Block.h"

struct Game {
    static const int sWidth = 800;
    static const int sHeight = 600;

    static const int wallNums = 4;
    static Rectangle walls[wallNums];

    static Ball ball;
    static Cannon cannon;

    static const int blockNums = 4;
    static Block blocks[blockNums];

    static float blockSpeedAdd;

    static int score;

    static Camera2D camera;
    static float shake;

    static float volume;

    static Texture2D pauseTex;
    static Texture2D retryTex;
    static Texture2D playTex;
    static Sound selectSfx;
    static Sound hitSfx;
    static Sound deathSfx;
    static Music bgm;

    static bool gameStarted;
    static bool pause;

    static void drawUpdateVolumeBar();

    static void drawMainMenu();
    static void drawPauseMenu();
    static void drawDeathMenu();

    static void init();
    static void update(float dt);
    static void draw();
    static void unload();
};

#endif