#include "GameScene.hpp"
#include "BulletManager/BulletManager.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include "Enemies/Chaser/Chaser.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include <memory>

void GameScene::Enter() {
  _sceneTransition = {SceneRequest::NONE, SceneID::NONE};
  _bulletManager = BulletManager();
  _entityManager = EntityManager();

  _entityManager.Init();
  _bulletManager.Init();

  _entityManager.SetBulletSpawnCallBack(
      [this](std::unique_ptr<Bullet> &bullet) {
        this->_bulletManager.SpawnBullet(bullet);
      });
  SpawnPlayer();
  SpawnWave();
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
  ClearBackground(BLACK);
  _entityManager.Render();
  _bulletManager.Render();
  DrawText(TextFormat("FPS: %i", GetFPS()), 30, 30, 48, WHITE);
}

void GameScene::Update(float dt, const RenderContext &rendercontext) {
  GetInput();
  _entityManager.Update(dt, rendercontext);
  _bulletManager.Update(dt, rendercontext);
}

SceneTransition GameScene::GetSceneTransition() { return _sceneTransition; }

void GameScene::SpawnPlayer() {
  _entityManager.SpawnPlayer(BLUE, 500.f, 3, Vector2{100, 100});
}

void GameScene::SpawnWave() {
  for (float i = 0; i < 1; ++i) {
    std::unique_ptr<Entity> enemy =
        std::make_unique<Chaser>(RED, 200.f, 2, Vector2{i * 100, i * 200});
    _entityManager.AddEntity(enemy);
  }
}
