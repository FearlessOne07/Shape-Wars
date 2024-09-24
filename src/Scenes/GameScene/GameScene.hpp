#include "Core/SceneManager/Scene.hpp"
#include "Core/SceneManager/SceneTransition.hpp"

class GameScene : public Scene {
  SceneTransition _sceneTransition;

public:
  void Update(float dt) override;
  void Render() override;
  void Enter() override;
  void Exit() override;
  SceneTransition GetSceneTransition() override;
};
