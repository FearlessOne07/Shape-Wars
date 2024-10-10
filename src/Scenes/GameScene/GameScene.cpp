#include "GameScene.hpp"
#include "BulletManager/BulletManager.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include "Enemies/Chaser/Chaser.hpp"
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

  _camera.zoom = 0.7;
  _camera.offset = {0, 0};

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
  BeginMode2D(_camera);
  DrawCircle(0, 0, 5, RED);
  _entityManager.Render();
  _bulletManager.Render();
  EndMode2D();
  DrawText(TextFormat("FPS: %i", GetFPS()), 30, 30, 48, WHITE);
}

void GameScene::Update(float dt, const RenderContext &rendercontext) {
  GetInput();
  UpdateCamera(dt, rendercontext);
  _entityManager.Update(dt, rendercontext);
  _bulletManager.Update(dt, rendercontext);
}

SceneTransition GameScene::GetSceneTransition() { return _sceneTransition; }

void GameScene::SpawnPlayer() {
  _entityManager.SpawnPlayer(BLUE, 500.f, 3, Vector2{100, 100});
}

void GameScene::UpdateCamera(float dt, const RenderContext &rendercontext) {
  _camera.offset.x = rendercontext.gameWidth / 2;
  _camera.offset.y = rendercontext.gameHeight / 2;
  rendercontext.camera = _camera;
}

void GameScene::SpawnWave() {
  for (float i = 1; i <= 0; ++i) {
    std::unique_ptr<Entity> enemy =
        std::make_unique<Chaser>(RED, 200.f, 2, Vector2{i * 100, i * 200});
    _entityManager.AddEntity(enemy);
  }
}
