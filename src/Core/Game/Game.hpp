#pragma once
#include "Core/SceneManager/SceneManager.hpp"
#include "raylib.h"

class Game {
private:
  bool _running;
  RenderTexture _renderTexture;
  float _gameWidth;
  float _gameHeight;

private: // Systems
  SceneManager _scenemanager;

public:
  Game() = default;
  void Init(int width, int height, const char *title, int fps = 0);
  void Run();
  void End();

  void QuitCallBack();
};
