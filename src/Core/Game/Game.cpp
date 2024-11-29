#include "Game.hpp"
#include "RenderContext.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "raylib.h"
#include <algorithm>
#include <date/date.h>

void Game::Init(int width, int height, const char *title, int fps) {

  // Initialize Raylib
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(1280, 720, title);
  SetExitKey(0);
  SetWindowMinSize(1280, 720);
  if (fps > 0) {
    SetTargetFPS(fps);
  }

  // Initia Render Texture
  _renderTexture = LoadRenderTexture(width, height);
  _gameWidth = _renderTexture.texture.width;
  _gameHeight = _renderTexture.texture.height;
  _running = true;

  // Init Systems
  _scenemanager = SceneManager();
  _scenemanager.SetQuitCallBack([this]() { this->Quit(); });
}

void Game::Run() {
  while (!WindowShouldClose() && _running) {

    // Get Input that affects whole game
    GetInput();

    // Initialize render context
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    float scale = std::min((float)windowWidth / _gameWidth,
                           (float)windowHeight / _gameHeight);
    int marginX = (windowWidth - (_gameWidth * scale)) / 2;
    int marginY = (windowHeight - (_gameHeight * scale)) / 2;
    RenderContext rendercontext = {_gameWidth, _gameHeight, (float)marginX,
                                   (float)marginY, scale};

    // Delta Time
    float dt = GetFrameTime();

    // Update Scene mamnager
    _scenemanager.Update(dt, rendercontext);

    // Begin rendering of Scenes
    BeginTextureMode(_renderTexture);
    _scenemanager.Render();
    EndTextureMode();

    // Draw Render texture to the Screen
    BeginDrawing();
    ClearBackground({33, 34, 39, 255});
    DrawTexturePro(_renderTexture.texture, {0, 0, _gameWidth, -_gameHeight},
                   {(float)marginX, (float)marginY, _gameWidth * scale,
                    _gameHeight * scale},
                   {0, 0}, 0.f, WHITE);
    EndDrawing();
  }

  // Cleanup
  End();
}
void Game::GetInput() {
  if (IsKeyPressed(KEY_F11)) {
    ToggleFullscreen();
  }
}
void Game::End() {
  UnloadRenderTexture(_renderTexture);
  CloseWindow();
}

void Game::Quit() { _running = false; }
void Game::RegisterScene(int sceneID, FactoryCallBack factory) {
  _scenemanager.RegisterScene(sceneID, std::move(factory));
}
