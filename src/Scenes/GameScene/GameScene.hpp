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

private:
  void UpdateCamera(float dt, const RenderContext& renderContext);
  void GetInput() override;
  void SpawnPlayer();
  void SpawnWave();

private:
public:
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
  void Enter() override;
  void Exit() override;
  SceneTransition GetSceneTransition() override;
};
