#include "GameScene.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include "Player/Player.hpp"
#include "raylib.h"

void GameScene::Enter() {
  _sceneTransition = {SceneRequest::NONE, SceneID::NONE};
  _player = std::make_unique<Player>(BLUE, 700, 3, Vector2{100, 100});
}

void GameScene::GetInput() {
  if (IsKeyPressed(KEY_ESCAPE)) {
    _sceneTransition = {SceneRequest::QUIT, SceneID::NONE};
  }
}
void GameScene::Exit() {}

void GameScene::Render() {
  ClearBackground(RED);
  DrawText(TextFormat("FPS: %i", GetFPS()), 30, 30, 48, WHITE);
  _player->Render();
}

void GameScene::Update(float dt) {
  GetInput();
  _player->Update(dt);
}

SceneTransition GameScene::GetSceneTransition() { return _sceneTransition; }
