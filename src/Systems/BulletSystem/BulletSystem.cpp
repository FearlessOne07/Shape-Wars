#include "BulletSystem.hpp"
#include "Components/ShootComponent/ShootComponent.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib/raymath.h"
#include <iostream>
#include <memory>
#include <vector>

void BulletSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities_shtcmp = entityManager->Query<ShootComponent>();

  for (auto &e : entities_shtcmp)
  {
    auto *transcmp = e->GetComponent<Base::TransformComponent>();
    auto *shtcmp = e->GetComponent<ShootComponent>();

    if (shtcmp->bulletFireTimer >= shtcmp->bulletFireRate)
    {
      if (shtcmp->IsFiring)
      {
        std::cout << "Firing\n";
        shtcmp->bulletFireTimer = 0.f;
        shtcmp->IsFiring = false;

        Base::Entity *bullet = entityManager->AddEntity();

        auto *shtTranscmp = bullet->AddComponent<Base::TransformComponent>();
        shtTranscmp->position = transcmp->position;

        auto *mvcmp = bullet->AddComponent<Base::MoveComponent>();
        mvcmp->targetVelocity = Vector2Subtract(shtcmp->target, transcmp->position);
        mvcmp->speed = shtcmp->bulletSpeed;
        mvcmp->acceleration = shtcmp->bulletSpeed;

        auto *shpcmp = bullet->AddComponent<Base::ShapeComponent>();
        shpcmp->points = 50;
        shpcmp->color = WHITE;
        shpcmp->radius = 20;
        shpcmp->fill = true;
      }
    }
    else
    {
      shtcmp->bulletFireTimer += dt;
    }
  }
}
