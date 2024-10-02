#include "Core/Game/Game.hpp"
#include <cstdlib>

int main(void) {
  Game game;
  game.Init(1920, 1080, "Shape Wars");
  game.Run();

  return 0;
}
