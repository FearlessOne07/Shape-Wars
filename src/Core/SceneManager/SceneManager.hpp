#pragma once
#include "Core/Game/RenderContext.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include "Scene.hpp"
#include <functional>
#include <memory>
#include <stack>

class SceneManager {

  // Type Defs
  using QuitCallBack = std::function<void()>;
  using FactoryCallBack = std::function<std::unique_ptr<Scene>()>;

private:
  QuitCallBack _quitCallBack;
  std::unordered_map<int, FactoryCallBack> _factories;

private:
  std::stack<std::unique_ptr<Scene>> _scenes;

  void PushScene(int sceneID, SceneData sceneData = SceneData());
  void ReplaceScene(int sceneId, SceneData sceneData = SceneData());
  void PopScene();

public:
  void RegisterSceneFactory(int sceneID, FactoryCallBack factory);
  void Update(float dt, const RenderContext &rendercontext);
  void Render();

  void SetQuitCallBack(QuitCallBack quitCallback);
};
