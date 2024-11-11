#include "GameScene.hpp"
#include "BulletManager/BulletManager.hpp"
#include "Core/Game/RenderContext.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include "EntityManager/Entity.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include <memory>

void GameScene::Enter() {

  // Init game scene systems
  _sceneTransition = {SceneRequest::NONE, SceneID::NONE};
  _bulletManager = BulletManager();
  _entityManager = EntityManager();

  _entityManager.Init();
  _bulletManager.Init();

  _entityManager.SetBulletSpawnCallBack(
      [this](std::unique_ptr<Bullet> &bullet) {
        this->_bulletManager.SpawnBullet(bullet);
      });

  _entityManager.SetGetBulletsCallBack(
      [this]() -> const std::vector<std::unique_ptr<Bullet>> & {
        return this->_bulletManager.GetBullets();
      });
  // Spawn the player
  _entityManager.SpawnPlayer();

  // Init camera
  _camera.zoom = 1.f;
  _camera.offset = {0, 0};
  _camera.target = {0, 0};

  _spawnDuration = 3.f;
  _spawnTimer = 0.f;
}

void GameScene::GetInput() {

  // Get input that affects this scene
  if (IsKeyPressed(KEY_ESCAPE)) {
    _sceneTransition = {SceneRequest::QUIT, SceneID::NONE};
  }
}

void GameScene::Exit() {
  // Reser entity and bullet manager classes on exit
  _entityManager.Reset();
  _bulletManager.Reset();
}

void GameScene::Render() {

  // Clear the screen
  ClearBackground(BLACK);

  // Debug
  DrawText(TextFormat("FPS: %i", GetFPS()), 10, 0, 40, WHITE);
  DrawText(TextFormat("Camera target: %i, %i", (int)_camera.target.x,
                      (int)_camera.offset.y),
           10, 50, 40, WHITE);
  DrawText(TextFormat("Player Position: %i, %i",
                      (int)_entityManager.GetPlayer()->GetPosition().x,
                      (int)_entityManager.GetPlayer()->GetPosition().y),
           10, 100, 40, WHITE);
  DrawText(
      TextFormat("Player Health: %i", (int)_entityManager.GetPlayer()->GetHp()),
      10, 150, 40, WHITE);

  // Being camera mode
  BeginMode2D(_camera);
  _entityManager.Render();
  _bulletManager.Render();
  EndMode2D();
}

void GameScene::Update(float dt, const RenderContext &rendercontext) {
  // Update the game scene objects
  GetInput();
  UpdateCamera(dt, rendercontext);
  _entityManager.Update(dt, rendercontext);
  _bulletManager.Update(dt, rendercontext);
}

// Scene transition request getter
SceneTransition GameScene::GetSceneTransition() { return _sceneTransition; }

void GameScene::UpdateCamera(float dt, const RenderContext &rendercontext) {

  // Update camera
  _camera.offset.x = rendercontext.gameWidth / 2;
  _camera.offset.y = rendercontext.gameHeight / 2;
  rendercontext.camera = _camera;
}
