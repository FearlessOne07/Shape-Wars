#include "SceneManager.hpp"
#include "Core/SceneManager/SceneTransition.hpp"
#include <iostream>
#include <utility>

void SceneManager::PushScene(std::unique_ptr<Scene> scene) {
  if (!_scenes.empty()) {
    // Exit the current scene
    _scenes.top()->Exit();
  }
  // Push new scen to the stack and enter it
  _scenes.push(std::move(scene));
  _scenes.top()->Enter();
}

void SceneManager::PopScene() {
  if (!_scenes.empty()) {

    // Exit the current scene and pop it off the stack
    _scenes.top()->Exit();
    _scenes.pop();
  }

  // Enter the scene below it if there is one
  if (!_scenes.empty()) {
    _scenes.top()->Enter();
  }
}

void SceneManager::ReplaceScene(std::unique_ptr<Scene> scene) {
  if (!_scenes.empty()) {

    // Exit the current scene and pop it
    _scenes.top()->Exit();
    _scenes.pop();
  }

  // Push the new scene and enter it
  _scenes.push(std::move(scene));
  _scenes.top()->Enter();
}

void SceneManager::SetQuitCallBack(QuitCallBack quitCallBack) {
  _quitCallBack = quitCallBack;
}

void SceneManager::Update(float dt, const RenderContext &rendercontext) {
  if (!_scenes.empty()) {

    // Update Current Scene
    _scenes.top()->Update(dt, rendercontext);

    // Check if the current scene has requested a scene change
    SceneTransition sceneTrans = _scenes.top()->GetSceneTransition();

    // Reset the scene's transisition request
    _scenes.top()->ResetSceneTransition();

    if (sceneTrans.request != SceneRequest::NONE) {
      if (sceneTrans.request == SceneRequest::QUIT) {
        std::cout << "Quiting!\n";

        // Quit if the scene requests to quite the game
        _quitCallBack();
      }
    }
  }
}

void SceneManager::Render() {
  if (!_scenes.empty()) {

    // Render current scene
    _scenes.top()->Render();
  }
}
