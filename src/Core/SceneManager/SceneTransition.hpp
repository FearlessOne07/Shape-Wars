#pragma once
enum class SceneID { NONE = 0, GAME_MENU };
enum class SceneRequest {
  NONE = 0,
  QUIT,
  PUSH_NEW_SCENE,
  POP_CURRENT_SCENE,
  REPLACE_CURRENT_SCENE
};

struct SceneTransition {
  SceneRequest request;
  SceneID sceneID;
};
