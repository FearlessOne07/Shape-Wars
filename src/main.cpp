#include "Scenes/GameScene.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "base/Game.hpp"
#include "base/systems/CameraSystem.hpp"
#include "base/systems/InputSystem.hpp"
#include "base/systems/MoveSystem.hpp"
#include "base/systems/RenderSystem.hpp"

int main()
{
  Base::Game game;
  game.Init(1920, 1080, "Ship Game");

  // Register Render  System
  game.RegisterSystem<Base::RenderSystem>(true);

  // Register Rest of systems; Systems are updated in order of registration
  game.RegisterSystem<Base::InputSystem>();
  game.RegisterSystem<BulletSystem>();
  game.RegisterSystem<Base::MoveSystem>();
  game.RegisterSystem<Base::CameraSystem>();

  // Register Scenes
  game.RegisterScene<GameScene>();

  // Run Game
  game.Run();
}
