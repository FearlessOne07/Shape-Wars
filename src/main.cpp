#include "Core/Game/Game.hpp"
#include "Scenes/GameScene/GameScene.hpp"
#include "Scenes/SceneID.hpp"
#include <cstdlib>

int main(void) {
  Game game;
  game.Init(1920, 1080, "Shape Wars");
  game.RegisterScene( //
      static_cast<int>(SceneID::GAME_SCENE),
      []() { return std::make_unique<GameScene>(); } //
  );
  game.Run();
  return 0;
}
