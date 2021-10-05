#include "raylib.h"

#include "GameUtils.h"
#include "Game.h"

int main(void) {
    InitWindow(Game::sWidth, Game::sHeight, "Cannon Breakout");
    InitAudioDevice();

    Texture2D vignetteVFX = LoadTexture("assets/vignette.png");

    Game::pauseTex = LoadTexture("assets/pause.png");
    Game::retryTex = LoadTexture("assets/retry.png");
    Game::playTex = LoadTexture("assets/play.png");
    Game::selectSfx = LoadSound("assets/select.ogg");
    Game::hitSfx = LoadSound("assets/hit.ogg");
    Game::deathSfx = LoadSound("assets/death.ogg");
    Game::bgm = LoadMusicStream("assets/bgm.ogg");

    Game::volume = 10.0f;

    Game::gameStarted = false;
    Game::ball.alive = true;

    PlayMusicStream(Game::bgm);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Game::update(GetFrameTime());

        BeginDrawing();
        ClearBackground(GUNMETAL);

        Game::draw();

        DrawTexture(vignetteVFX, 0, 0, Fade(WHITE, 0.3f));

        EndDrawing();
    }

    Game::unload();

    UnloadTexture(vignetteVFX);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}