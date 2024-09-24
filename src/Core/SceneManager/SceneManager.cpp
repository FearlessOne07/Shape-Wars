#include "SceneManager.hpp"
#include <utility>
void SceneManager::PushScene(std::unique_ptr<Scene> scene) {
  if (!_scenes.empty()) {
    _scenes.top()->Exit();
    _scenes.pop();
  }
  _scenes.push(std::move(scene));
  _scenes.top()->Enter();
}

void SceneManager::PopScene() {
  if (!_scenes.empty()) {
    _scenes.top()->Exit();
    _scenes.pop();
  }
  if (!_scenes.empty()) {
    _scenes.top()->Enter();
  }
}

void SceneManager::ReplaceScene(std::unique_ptr<Scene> scene) {
  if (!_scenes.empty()) {
    _scenes.top()->Exit();
    _scenes.pop();
  }
  _scenes.push(std::move(scene));
  _scenes.top()->Enter();
}

void SceneManager::SetQuitCallBack(QuitCallBack quitCallBack) {
  _quitCallBack = quitCallBack;
}

void SceneManager::Update(float dt) {
  if (!_scenes.empty()) {
    _scenes.top()->Update(dt);
  }
}
void SceneManager::Render() {
  if (!_scenes.empty()) {
    _scenes.top()->Render();
  }
}
