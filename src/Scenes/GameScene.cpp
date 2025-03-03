#include "GameScene.hpp"
#include "Components/RotationComponent/RotationComponent.hpp"
#include "Components/ShootComponent/ShootComponent.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
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
#include "base/systems/CameraSystem.hpp"
#include "base/systems/InputSystem.hpp"
#include "base/systems/MoveSystem.hpp"
#include "base/systems/RenderSystem.hpp"
#include "raylib.h"
#include "raylib/raylib.h"
#include <cstdlib>

void GameScene::Enter( //
  Base::SystemManager *systemManager, Base::AssetManager *assetManager,
  Base::SceneData sceneData //
)
{
  systemManager->ActivatSystem<Base::MoveSystem>();
  systemManager->ActivatSystem<Base::InputSystem>();
  systemManager->ActivatSystem<Base::RenderSystem>();
  systemManager->ActivatSystem<Base::CameraSystem>();
  systemManager->ActivatSystem<RotationSystem>();
  systemManager->ActivatSystem<BulletSystem>();

  SpawnPlayer(assetManager);

  const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
  rd->camera.target = {.x = 0, .y = 0};
  rd->camera.zoom = 0.8;
  rd->camera.offset = {.x = rd->gameWidth / 2, .y = rd->gameHeight / 2};
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
  BeginMode2D(rd->camera);
  DrawRectangleLinesEx({-1000, -1000, 2000, 2000}, 5, WHITE);
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

  auto *shpcmp = e->AddComponent<Base::ShapeComponent>();
  shpcmp->fill = true;
  shpcmp->color = WHITE;
  shpcmp->points = 7;
  shpcmp->radius = 40;

  // Input comp registeration
  auto *mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->speed = 1000.f;
  mvcmp->acceleration = 3;

  auto *rotcmp = e->AddComponent<RotationComponent>();
  rotcmp->rotationAcceleration = 2;
  rotcmp->targetRotationVelocity = 1;
  rotcmp->rotationSpeed = 250.f;

  auto *shtcmp = e->AddComponent<ShootComponent>();
  shtcmp->bulletFireRate = 0.5;
  shtcmp->bulletLifetime = 3;
  shtcmp->bulletSpeed = 900.f;

  auto *inpcmp = e->AddComponent<Base::InputComponent>();
  inpcmp->BindKeyPressed(KEY_A, [mvcmp]() { mvcmp->targetVelocity.x = -1; });
  inpcmp->BindKeyPressed(KEY_D, [mvcmp]() { mvcmp->targetVelocity.x = 1; });
  inpcmp->BindKeyPressed(KEY_W, [mvcmp]() { mvcmp->targetVelocity.y = -1; });
  inpcmp->BindKeyPressed(KEY_S, [mvcmp]() { mvcmp->targetVelocity.y = 1; });

  inpcmp->BindKeyReleased(KEY_A, [mvcmp]() { mvcmp->targetVelocity.x = 0; });
  inpcmp->BindKeyReleased(KEY_D, [mvcmp]() { mvcmp->targetVelocity.x = 0; });
  inpcmp->BindKeyReleased(KEY_W, [mvcmp]() { mvcmp->targetVelocity.y = 0; });
  inpcmp->BindKeyReleased(KEY_S, [mvcmp]() { mvcmp->targetVelocity.y = 0; });

  inpcmp->BindMouseButtonPressed(MOUSE_BUTTON_LEFT, [shtcmp]() {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    shtcmp->IsFiring = true;
    shtcmp->target = GetScreenToWorld2D(rd->GetScreenToGame(GetMousePosition()), rd->camera);
  });

  auto *camcmp = e->AddComponent<Base::CameraComponent>();
  camcmp->cameraMode = Base::CameraMode::SMOOTH_FOLLOW;
  camcmp->maxFollowDistance = 100.f;
  camcmp->cameraSpeed = 1000.f;
}
