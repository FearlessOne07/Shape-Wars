#pragma once
#include "Core/SceneManager/SceneManager.hpp"
#include "raylib.h"

class Game {
  // Type defs
  using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

private:
  bool _running;
  RenderTexture _renderTexture;
  float _gameWidth;
  float _gameHeight;

private: // Systems
  SceneManager _scenemanager;

private: // Methods
  void GetInput();
  void Quit();
  void End();

public:
  Game() = default;
  void Init(int width, int height, const char *title, int fps = 0);
  void RegisterScene(int sceneID, FactoryCallBack factory);
  void Run();
};
