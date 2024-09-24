#include "GameScene.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include "raylib.h"

void GameScene::Enter() {
  _sceneTransition = {SceneRequest::NULL, SceneID::NONE};
}

void GameScene::Exit() {}

void GameScene::Render() {
  ClearBackground(RED);

  DrawRectangle(100, 100, 100, 90, BLUE);
}

void GameScene::Update(float dt) {}

SceneTransition GameScene::GetSceneTransition() { return _sceneTransition; }
