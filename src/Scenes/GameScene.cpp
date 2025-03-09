#include "GameScene.hpp"
#include "Components/ShootComponent/ShootComponent.hpp"
#include "Systems/BulletSystem/BulletSystem.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/RenderContext.hpp"
#include "base/RenderContextSingleton.hpp"
#include "base/SystemManager.hpp"
#include "base/components/BoundingBoxComponent.hpp"
#include "base/components/CameraComponent.hpp"
#include "base/components/GravityComponent.hpp"
#include "base/components/InputComponent.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/TransformComponent.hpp"
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

  auto *transcmp = e->GetComponent<Base::TransformComponent>();
  transcmp->position.y = -1000 - 80;

  // Input comp registeration
  auto *mvcmp = e->AddComponent<Base::MoveComponent>();
  mvcmp->speed = 1000.f;
  mvcmp->acceleration = 3;

  auto *shtcmp = e->AddComponent<ShootComponent>();
  shtcmp->bulletFireRate = 0.5;
  shtcmp->bulletLifetime = 3;
  shtcmp->bulletSpeed = 2000.f;

  auto *abbcmp = e->AddComponent<Base::BoundingBoxComponent>();
  abbcmp->size = {.x = 40, .y = 80};
  abbcmp->positionOffset = {.x = 0, .y = 0};
  abbcmp->draw = true;
  abbcmp->fill = false;
  abbcmp->color = WHITE;

  auto gravcmp = e->AddComponent<Base::GravityComponent>();
  gravcmp->gravityStrength = 4000.f;
  gravcmp->jumpHeight = 1500.f;

  auto *inpcmp = e->AddComponent<Base::InputComponent>();
  inpcmp->BindKeyPressed(KEY_A, [mvcmp]() { mvcmp->targetVelocity.x = -1; });
  inpcmp->BindKeyPressed(KEY_D, [mvcmp]() { mvcmp->targetVelocity.x = 1; });
  inpcmp->BindKeyDown(KEY_SPACE, [gravcmp, mvcmp]() {
    if (!gravcmp->isJumping)
    {
      gravcmp->isJumping = true;
      mvcmp->velocity.y = -gravcmp->jumpHeight;
    }
  });

  inpcmp->BindKeyReleased(KEY_A, [mvcmp]() { mvcmp->targetVelocity.x = 0; });
  inpcmp->BindKeyReleased(KEY_D, [mvcmp]() { mvcmp->targetVelocity.x = 0; });

  inpcmp->BindMouseButtonDown(MOUSE_BUTTON_LEFT, [shtcmp]() {
    const Base::RenderContext *rd = Base::RenderContextSingleton::GetInstance();
    shtcmp->IsFiring = true;
    shtcmp->target = GetScreenToWorld2D(rd->GetScreenToGame(GetMousePosition()), rd->camera);
  });

  auto *camcmp = e->AddComponent<Base::CameraComponent>();
  camcmp->cameraMode = Base::CameraMode::SMOOTH_FOLLOW;
  camcmp->maxFollowDistance = 200.f;
  camcmp->cameraSpeed = 1000.f;

  // Obstacle
  Base::Entity *obs = GetEntityManager()->AddEntity();
  auto *transcmpObs = obs->GetComponent<Base::TransformComponent>();
  transcmpObs->position = {.x = -1000, .y = -1000};

  auto *abbcmpObst = obs->AddComponent<Base::BoundingBoxComponent>();
  abbcmpObst->size = {.x = 2000, .y = 2000};
  abbcmpObst->draw = true;
  abbcmpObst->fill = false;
  abbcmpObst->nonFillThickness = 5;
  abbcmpObst->color = WHITE;
}
