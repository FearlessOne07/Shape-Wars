#include "GameScene.hpp"
#include "BulletManager/BulletManager.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include <memory>

void GameScene::Enter() {
  _sceneTransition = {SceneRequest::NONE, SceneID::NONE};
  _bulletManager = BulletManager();
  _entityManager = EntityManager();
  _entityManager.SetBulletSpawnCallBack(
      [this](std::unique_ptr<Bullet> &bullet) {
        this->_bulletManager.SpawnBullet(bullet);
      });
  SpawnPlayer();
}

void GameScene::GetInput() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    _sceneTransition = {SceneRequest::QUIT, SceneID::NONE};
  }
}
void GameScene::Exit() {
  _entityManager.Reset();
  _bulletManager.Reset();
}

void GameScene::Render() {
  ClearBackground(RED);
  _entityManager.Render();
  _bulletManager.Render();
  DrawText(TextFormat("FPS: %i", GetFPS()), 30, 30, 48, WHITE);
}

void GameScene::Update(float dt) {
  GetInput();
  _entityManager.Update(dt);
  _bulletManager.Update(dt);
}

SceneTransition GameScene::GetSceneTransition() { return _sceneTransition; }

void GameScene::SpawnPlayer() {
  std::unique_ptr<Entity> player =
      std::make_unique<Player>(BLUE, 500.f, 3, Vector2{100, 100});
  _entityManager.AddEntity(player);
}
