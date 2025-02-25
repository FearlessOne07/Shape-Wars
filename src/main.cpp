#include "Scenes/GameScene.hpp"
#include "Systems/RotationSystem/RotationSystem.hpp"
#include "base/Game.hpp"
#include "base/systems/CameraSystem.hpp"
#include "base/systems/InputSystem.hpp"
#include "base/systems/MoveSystem.hpp"
#include "base/systems/RenderSystem.hpp"

int main(void)
{
  Base::Game game;
  game.Init(1920, 1080, "Ship Game");

  // Register Systems
  game.RegisterSystem<Base::RenderSystem>(true);
  game.RegisterSystem<Base::InputSystem>();
  game.RegisterSystem<RotationSystem>();
  game.RegisterSystem<Base::MoveSystem>();
  game.RegisterSystem<Base::CameraSystem>();

  // Register Scenes
  game.RegisterScene<GameScene>();

  // Run Game
  game.Run();
}
