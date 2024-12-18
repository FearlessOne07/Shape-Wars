#include "Core/Game/Game.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/SceneID.hpp"

#ifdef __cplusplus
extern "C"
{
#endif

  __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif

int main(void)
{
  Game game;
  game.Init(1920, 1080, "Shape Wars");
  game.RegisterScene(static_cast<int>(SceneID::GAME_SCENE), []() { return std::make_unique<GameScene>(); });
  game.Run();
  return 0;
}
