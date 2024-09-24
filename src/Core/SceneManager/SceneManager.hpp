#pragma once
#include "Scene.hpp"
#include <functional>
#include <memory>
#include <stack>

class SceneManager {
  using QuitCallBack = std::function<void()>;
  QuitCallBack _quitCallBack;

private:
  std::stack<std::unique_ptr<Scene>> _scenes;

public:
  void PushScene(std::unique_ptr<Scene> scene);
  void PopScene();
  void ReplaceScene(std::unique_ptr<Scene> scene);

  void Update(float dt);
  void Render();

  void SetQuitCallBack(QuitCallBack quitCallback);
};
