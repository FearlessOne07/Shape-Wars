#include "GameScene.hpp"
#include "Components/RotationComponent/RotationComponent.hpp"
#include "Systems/RotationSystem/RotationSystem.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/SystemManager.hpp"
#include "base/components/CameraComponent.hpp"
#include "base/components/InputComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "base/systems/CameraSystem.hpp"
#include "base/systems/InputSystem.hpp"
#include "base/systems/MoveSystem.hpp"
#include "base/systems/RenderSystem.hpp"
#include "raylib.h"
#include <cstdlib>
#include <iostream>

void GameScene::Enter( //
  Base::SystemManager *systemManager, Base::AssetManager *assetManager,
  Base::SceneData sceneData //
)
{
  systemManager->ActivatSystem<Base::MoveSystem>();
  systemManager->ActivatSystem<Base::InputSystem>();
  systemManager->ActivatSystem<Base::RenderSystem>();
  systemManager->ActivatSystem<RotationSystem>();
  systemManager->ActivatSystem<Base::CameraSystem>();

  SpawnPlayer(assetManager);

  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  rd->camera.target = {0, 0};
  rd->camera.zoom = 1;
  rd->camera.offset = {rd->gameWidth / 2, rd->gameHeight / 2};
}

void GameScene::GetInput()
{
}

void GameScene::Update(float dt, Base::SystemManager *systemManager)
{
  systemManager->Update(dt);
}

void GameScene::Render(Base::SystemManager *systemManager)
{
  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();

  DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 30, WHITE);
  std::cout << rd->camera.target.x << "," << rd->camera.target.y << "\n";
  BeginMode2D(rd->camera);
  systemManager->Render();
  EndMode2D();
}

void GameScene::Exit(Base::SystemManager *systemManager, Base::AssetManager *assetManager)
{
  systemManager->DeactivateActiveSystems();
}

void GameScene::SpawnPlayer(Base::AssetManager *assetManager)
{
  Base::Entity *e = GetEntityManager()->AddEntity();

  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();

  Base::TransformComponent *transcmp = e->AddComponent<Base::TransformComponent>();

  Base::ShapeComponent *shpcmp = e->AddComponent<Base::ShapeComponent>();
  shpcmp->fill = true;
  shpcmp->color = WHITE;
  shpcmp->points = 7;
  shpcmp->radius = 40;

  // Input comp registeration
  Base::MoveComponent *mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->speed = 1000.f;
  mvcmp->acceleration = 3;

  RotationComponent *rotcmp = e->AddComponent<RotationComponent>();
  rotcmp->rotationAcceleration = 2;
  rotcmp->targetRotationVelocity = 1;
  rotcmp->rotationSpeed = 250.f;

  Base::InputComponent *inpcmp = e->AddComponent<Base::InputComponent>();
  inpcmp->BindKeyPressed(KEY_A, [mvcmp]() { mvcmp->targetVelocity.x = -1; });
  inpcmp->BindKeyPressed(KEY_D, [mvcmp]() { mvcmp->targetVelocity.x = 1; });
  inpcmp->BindKeyPressed(KEY_W, [mvcmp]() { mvcmp->targetVelocity.y = -1; });
  inpcmp->BindKeyPressed(KEY_S, [mvcmp]() { mvcmp->targetVelocity.y = 1; });

  inpcmp->BindKeyReleased(KEY_A, [mvcmp]() { mvcmp->targetVelocity.x = 0; });
  inpcmp->BindKeyReleased(KEY_D, [mvcmp]() { mvcmp->targetVelocity.x = 0; });
  inpcmp->BindKeyReleased(KEY_W, [mvcmp]() { mvcmp->targetVelocity.y = 0; });
  inpcmp->BindKeyReleased(KEY_S, [mvcmp]() { mvcmp->targetVelocity.y = 0; });

  Base::CameraComponent *camcmp = e->AddComponent<Base::CameraComponent>();
  camcmp->cameraMode = Base::CameraMode::SMOOTH_FOLLOW;
}
