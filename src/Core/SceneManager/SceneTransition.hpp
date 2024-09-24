#pragma once
enum SceneID { NONE = 0, GAME_MENU };
enum SceneRequest {
  NULL = 0,
  QUIT,
  PUSH_NEW_SCENE,
  POP_CURRENT_SCENE,
  REPLACE_CURRENT_SCENE
};

struct SceneTransition {
  SceneRequest requrest;
  SceneID sceneID;
};
