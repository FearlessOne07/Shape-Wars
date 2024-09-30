#include "SceneManager.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include <iostream>
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

void SceneManager::Update(float dt, const RenderContext &rendercontext) {
  if (!_scenes.empty()) {
    _scenes.top()->Update(dt, rendercontext);

    SceneTransition sceneTrans = _scenes.top()->GetSceneTransition();
    if (sceneTrans.request != SceneRequest::NONE) {
      if (sceneTrans.request == SceneRequest::QUIT) {
        std::cout << "Quiting!\n";
        _quitCallBack();
      }

      _scenes.top()->ResetSceneTransition();
    }
  }
}

void SceneManager::Render() {
  if (!_scenes.empty()) {
    _scenes.top()->Render();
  }
}
