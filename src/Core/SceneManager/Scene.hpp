#pragma once
#include "SceneTransition.hpp"

class Scene {
public:
  virtual ~Scene() {}
  virtual void Update(float dt) = 0;
  virtual void Render() = 0;
  virtual void Enter() = 0;
  virtual void Exit() = 0;

  virtual SceneTransition GetSceneTransition() {
    return {SceneRequest::NULL, SceneID::NONE};
  }
};
