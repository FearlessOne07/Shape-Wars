#include "Core/EntityManager/Entity.hpp"
#include "Core/SceneManager/Scene.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include <memory>

class GameScene : public Scene {
private:
  std::unique_ptr<Entity> _player;

private:
  void GetInput() override;

private:
public:
  void Update(float dt) override;
  void Render() override;
  void Enter() override;
  void Exit() override;
  SceneTransition GetSceneTransition() override;
};
