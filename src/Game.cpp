#include "Game.h"
#include "Utils.h"
#include <math.h>

Rectangle Game::walls[wallNums] = {
    { 0, 0, sWidth, 10 },
    { 0, sHeight - 10, sWidth, 10 },
    { 0, 10, 10, sHeight - 20 },
    { sWidth - 10, 10, 10, sHeight - 20 }
};

Ball Game::ball;
Cannon Game::cannon;

Block Game::blocks[blockNums] = {};
float Game::blockSpeedAdd;

int Game::score;

Camera2D Game::camera;
float Game::shake;

Texture2D Game::pauseTex;
Texture2D Game::retryTex;
Texture2D Game::playTex;
Sound Game::selectSfx;
Sound Game::hitSfx;
Sound Game::deathSfx;
Music Game::bgm;

float Game::volume;

bool Game::pause;
bool Game::gameStarted;

void Game::drawUpdateVolumeBar() {
    if (IsKeyPressed(KEY_LEFT) && volume > 0) volume--;
    else if (IsKeyPressed(KEY_RIGHT) && volume < 10) volume++;
    SetMasterVolume(volume / 10.0f);

    for (int i = 0; i < 10; i++) {
        if (i < volume) DrawRectangle(295 + 21 * i, 550, 20, 20, DARKGRAY);
        DrawRectangleLines(295 + 21 * i, 550, 20, 20, BLACK);
    }

    DrawText("[LEFT]          VOLUME          [RIGHT]",
        sWidth / 2.0f - MeasureText("[LEFT]          VOL", 10), 580,
        10, RAYWHITE
    );
}

void Game::drawMainMenu() {
    DrawRectangle(0, 0, sWidth, sHeight, { 130, 130, 130, 130 });
    DrawText("Cannon Breakout", sWidth / 2 - MeasureText("Cannon Breakout", 50) / 2, 150, 50, RAYWHITE);
    DrawTextureEx(
        playTex,
        { sWidth / 2.0f - playTex.width * 2.5f, sHeight / 2.0f - playTex.height * 2.5f + 50 },
        0, 5.0f, WHITE
    );
    DrawText("[ENTER]",
        sWidth / 2.0f - MeasureText("[ENTER]", 15) / 2.0f,
        sHeight / 2.0f - playTex.height * 2.5f + 100,
        15, RAYWHITE
    );

    drawUpdateVolumeBar();
}

void Game::drawPauseMenu() {
    DrawRectangle(0, 0, sWidth, sHeight, Fade(GRAY, 0.5f));
    DrawText("Paused", sWidth / 2 - MeasureText("Paused", 50) / 2, 150, 50, RAYWHITE);
    DrawTextureEx(playTex,
        { (sWidth / 2.0f - playTex.width * 5.0f) - 50, sHeight / 2.0f - playTex.height * 2.5f + 50 },
        0, 5, WHITE
    );
    DrawTextureEx(retryTex,
        { sWidth / 2.0f + 50, sHeight / 2.0f - retryTex.height * 2.5f + 50 },
        0, 5.0f, WHITE
    );
    DrawText("[P]",
        ((sWidth / 2.0f - playTex.width * 2.5f) - 50) - MeasureText("[P]", 15) / 2.0f,
        sHeight / 2.0f - playTex.height * 2.5f + 100,
        15, RAYWHITE
    );
    DrawText("[R]",
        sWidth / 2.0f + retryTex.width * 2.5f + 50 - MeasureText("[R]", 15) / 2.0f,
        sHeight / 2.0f - retryTex.height * 2.5f + 100,
        15, RAYWHITE
    );

    drawUpdateVolumeBar();
}

void Game::drawDeathMenu() {
    DrawRectangle(0, 0, sWidth, sHeight, Fade(GRAY, 0.5f));
    DrawText("Yuo ded", sWidth / 2 - MeasureText("Yuo ded", 50) / 2, 150, 50, RAYWHITE);
    DrawText(TextFormat("Score: %i", score),
        sWidth / 2.0f - MeasureText(TextFormat("Score: %i", score), 20) / 2.0f,
        195, 20, RAYWHITE
    );
    DrawTextureEx(retryTex,
        { sWidth / 2.0f - retryTex.width * 2.5f, sHeight / 2.0f - retryTex.height * 2.5f + 50 },
        0, 5.0f, WHITE
    );
    DrawText("[R]",
        sWidth / 2.0f - MeasureText("[R]", 15) / 2.0f,
        sHeight / 2.0f - retryTex.height * 2.5f + 100,
        15, RAYWHITE
    );

    drawUpdateVolumeBar();
}

void Game::init() {
    pause = false;

    score = 0;

    shake = 0;

    camera.target = { sWidth / 2.0f, sHeight / 2.0f };
    camera.offset = { sWidth / 2.0f, sHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    ball.init();
    cannon.init();

    blockSpeedAdd = 0;

    for (int i = 0; i < blockNums; i++) {
        blocks[i].active = true;
        blocks[i].rec = { (float)GetRandomValue(20.0f, 154*4), 50.0f + 50.0f*i, 144, 40 };
        blocks[i].vel = 10;
    }
}

void Game::update(float dt) {
    UpdateMusicStream(bgm);

    if (!gameStarted) return;

    if (shake != 0) {
        shake -= dt;

        if (shake < 0) shake = 0;

        camera.rotation = 30.0f * powf(shake, 2.0f) * (GetRandomValue(-100, 100) / 100.0f);
        camera.offset.x = sWidth / 2.0f + 50.0f * powf(shake, 2.0f) * (GetRandomValue(-100, 100) / 100.0f);
        camera.offset.y = sHeight / 2.0f + 50.0f * powf(shake, 2.0f) * (GetRandomValue(-100, 100) / 100.0f);
    } else {
        camera.rotation = 0.0f;
        camera.offset.x = sWidth / 2.0f;
        camera.offset.y = sHeight / 2.0f;
    }

    if (IsKeyPressed(KEY_P) && ball.alive) {
        pause = !pause;
        PlaySound(selectSfx);
    }
    if (IsKeyPressed(KEY_R)){
        init();
        PlaySound(selectSfx);
    }
    if (pause || !ball.alive) return;

    ball.update(dt);
    cannon.update(dt);

    int destroyedBlocks = 0;
    int velMultiplier = blockNums+1;
    for (int i = 0; i < blockNums; i++) {
        velMultiplier--;

        if ((blocks[i].rec.x + blocks[i].vel*dt) < 10 ||
            (blocks[i].rec.x + blocks[i].vel*dt + blocks[i].rec.width) > Game::sWidth-10)
            blocks[i].vel = blocks[i].vel*-1;

        if (blocks[i].active) {
            blocks[i].rec.x += blocks[i].vel*dt*velMultiplier/2 + blockSpeedAdd/(blocks[i].vel/10);
        } else destroyedBlocks++;
    }

    if (destroyedBlocks >= blockNums) {
        for (int i = 0; i < blockNums; i++) {
            blocks[i].active = true;
            blocks[i].rec.x = (float)GetRandomValue(20.0f, 154*4);
            blocks[i].vel = 10;
        }
        ball.circle.x = sWidth/2.0f;
        ball.circle.y = 500;
        cannon.pos.x = sWidth/2.0f;
        blockSpeedAdd += 0.5f;
        score++;
    }
}

void Game::draw() {
    if (!gameStarted) {
        if (IsKeyPressed(KEY_ENTER)) {
            gameStarted = true;
            init();
            PlaySound(selectSfx);
        }
        drawMainMenu();
        return;
    }
    
    BeginMode2D(camera);

    for (int i = 0; i < wallNums; i++) {
        DrawRectangleRec(walls[i], (i<2)? MEDIUMTURQUOISE : INDIGODYE);
    }

    for (int i = 0; i < blockNums; i++) {
        DrawLineEx({10, 70.0f + 50.0f*i}, {sWidth-10, 70.0f + 50.0f*i}, 5, PRUSSIANBLUE);
        if (blocks[i].active) DrawRectangleRec(blocks[i].rec, INDIGODYE);
    }

    DrawLineEx({10, cannon.pos.y}, {sWidth-10, cannon.pos.y}, 5, PRUSSIANBLUE);

    ball.draw();
    cannon.draw();

    EndMode2D();

    DrawText(TextFormat("Score: %i", score), 5, 5, 20, RAYWHITE);

    if (pause) drawPauseMenu();
    else if (!ball.alive) drawDeathMenu();
    else {
        DrawTextureEx(pauseTex, { (sWidth - pauseTex.width * 5.0f) - 5, 5 }, 0, 5, WHITE);
        DrawText("[P]",
            (sWidth - pauseTex.width*2.5f) - 5 - MeasureText("[P]", 15) / 2.0f,
            pauseTex.height * 5.0f + 10,
            15, RAYWHITE
        );
        DrawTextureEx(retryTex, { (sWidth - pauseTex.width*5.0f - 50) - 5, 5 }, 0, 5, WHITE);
        DrawText("[R]",
            (sWidth - pauseTex.width*2.5f) - 5 - MeasureText("[R]", 15)/2.0f - 50,
            pauseTex.height * 5.0f + 10,
            15, RAYWHITE
        );
    }
}

void Game::unload() {
    ball.unload();
    cannon.unload();
    UnloadTexture(pauseTex);
    UnloadTexture(retryTex);
    UnloadTexture(playTex);
    UnloadSound(selectSfx);
    UnloadSound(hitSfx);
    UnloadSound(deathSfx);
    UnloadMusicStream(bgm);
}
