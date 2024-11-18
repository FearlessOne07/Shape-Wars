#pragma once
#include "Core/Game/RenderContext.hpp"
#include "SceneTransition.hpp"
#include "raylib.h"

class Scene {
protected:
  virtual void GetInput() = 0;
  SceneTransition _sceneTransition;
  Color _fillColor;

public:
  virtual ~Scene() {}
  virtual void Update(float dt, const RenderContext &rendercontext) = 0;
  virtual void Enter(SceneData sceneData = SceneData()) = 0;
  virtual void Render() = 0;
  virtual void Exit() = 0;

  SceneTransition GetSceneTransition() { return _sceneTransition; }

  void ResetSceneTransition() { _sceneTransition = {SceneRequest::NONE, -1}; }
};
