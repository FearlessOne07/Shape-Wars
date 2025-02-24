#include "BulletSystem.hpp"
#include "Components/BulletComponent/BulletComponent.hpp"
#include "base/Entity.hpp"
#include "base/EntityManager.hpp"
#include "base/components/MoveComponent.hpp"
#include "base/components/ShapeComponent.hpp"
#include "base/components/TransformComponent.hpp"
#include "raymath.h"
#include <memory>
#include <vector>

void BulletSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities_bulcmp = entityManager->Query<BulletComponent>();

  for (auto &e : entities_bulcmp)
  {
    Base::TransformComponent *transcmp = e->GetComponent<Base::TransformComponent>();
    BulletComponent *bulcmp = e->GetComponent<BulletComponent>();

    if (bulcmp->bulletFireTimer >= bulcmp->bulletFireRate)
    {
      if (bulcmp->IsFiring)
      {
        bulcmp->bulletFireTimer = 0.f;
        bulcmp->IsFiring = false;
        Base::Entity *bullet = entityManager->AddEntity();

        Base::MoveComponent *mvcmp = bullet->AddComponent<Base::MoveComponent>();
        mvcmp->targetVelocity = Vector2Subtract(bulcmp->target, transcmp->position);
        mvcmp->speed = bulcmp->bulletSpeed;
        mvcmp->acceleration = bulcmp->bulletSpeed;

        Base::ShapeComponent *shpcmp = bullet->AddComponent<Base::ShapeComponent>();
      }
    }
    else
    {
      bulcmp->bulletFireTimer += dt;
    }
  }
}
