#include "BulletSystem.hpp"
#include "Components/BulletComponent/BulletComponent.hpp"
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
  std::vector<std::shared_ptr<Base::Entity>> entities_bulcmp = entityManager->Query<BulletComponent>();

  for (auto &e : entities_bulcmp)
  {
    auto *transcmp = e->GetComponent<Base::TransformComponent>();
    auto *bulcmp = e->GetComponent<BulletComponent>();

    if (bulcmp->bulletFireTimer >= bulcmp->bulletFireRate)
    {
      if (bulcmp->IsFiring)
      {
        std::cout << "Firing\n";
        bulcmp->bulletFireTimer = 0.f;
        bulcmp->IsFiring = false;

        Base::Entity *bullet = entityManager->AddEntity();
        auto *mvcmp = bullet->AddComponent<Base::MoveComponent>();
        mvcmp->targetVelocity = Vector2Subtract(bulcmp->target, transcmp->position);
        mvcmp->speed = bulcmp->bulletSpeed;
        mvcmp->acceleration = bulcmp->bulletSpeed;

        auto *shpcmp = bullet->AddComponent<Base::ShapeComponent>();
        shpcmp->points = 50;
        shpcmp->color = WHITE;
        shpcmp->radius = 20;
        shpcmp->fill = true;
      }
    }
    else
    {
      bulcmp->bulletFireTimer += dt;
    }
  }
}
