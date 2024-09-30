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
  virtual void Render() = 0;
  virtual void Enter() = 0;
  virtual void Exit() = 0;

  virtual SceneTransition GetSceneTransition() {
    return {SceneRequest::NONE, SceneID::NONE};
  }

  void ResetSceneTransition() {
    _sceneTransition = {SceneRequest::NONE, SceneID::NONE};
  }
};
