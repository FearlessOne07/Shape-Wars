#include "Game.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "raylib.h"
#include <algorithm>
#include <memory>

void Game::Init(int width, int height, const char *title, int fps) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(1280, 720, title);
  SetExitKey(0);
  SetWindowMinSize(1280, 720);

  if (fps > 0) {
    SetTargetFPS(fps);
  }

  _renderTexture = LoadRenderTexture(width, height);
  _gameWidth = _renderTexture.texture.width;
  _gameHeight = _renderTexture.texture.height;
  _running = true;

  // Init Systems
  _scenemanager = SceneManager();
  _scenemanager.SetQuitCallBack([this]() { this->QuitCallBack(); });
  _scenemanager.PushScene(std::make_unique<GameScene>());
}

void Game::Run() {

  while (!WindowShouldClose() && _running) {

    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    float scale =
        std::min(windowWidth / _gameWidth, windowHeight / _gameHeight);

    int marginX = (windowWidth - (_gameWidth * scale)) / 2;
    int marginY = (windowHeight - (_gameHeight * scale)) / 2;

    float dt = GetFrameTime();

    _scenemanager.Update(dt);

    BeginTextureMode(_renderTexture);
    _scenemanager.Render();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(_renderTexture.texture, {0, 0, _gameWidth, -_gameHeight},
                   {(float)marginX, (float)marginY, _gameWidth * scale,
                    _gameHeight * scale},
                   {0, 0}, 0.f, WHITE);
    EndDrawing();
  }
  End();
}
void Game::End() { CloseWindow(); }
void Game::QuitCallBack() { _running = false; }
