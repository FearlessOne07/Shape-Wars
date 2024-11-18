#include "BulletManager/BulletManager.hpp"
#include "Core/Game/RenderContext.hpp"
#include "Core/SceneManager/Scene.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include "EntityManager/EntityManager.hpp"
#include "raylib.h"

class GameScene : public Scene {
private:
  EntityManager _entityManager;
  BulletManager _bulletManager;
  Camera2D _camera;

  // Wave management
  float _spawnTimer;
  float _spawnDuration;

private:
  void UpdateCamera(float dt, const RenderContext &renderContext);
  void GetInput() override;
  void SpawnPlayer();

private:
public:
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
  void Enter(SceneData sceneData = SceneData()) override;
  void Exit() override;
};
