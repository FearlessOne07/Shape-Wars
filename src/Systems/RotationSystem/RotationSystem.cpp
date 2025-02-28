#include "RotationSystem.hpp"
#include "Components/RotationComponent/RotationComponent.hpp"
#include "base/EntityManager.hpp"
#include "base/components/TransformComponent.hpp"
#include "raylib/raymath.h"
#include <memory>
#include <vector>

void RotationSystem::Update(float dt, Base::EntityManager *entityManager)
{
  std::vector<std::shared_ptr<Base::Entity>> entities_rotcmp = entityManager->Query<RotationComponent>();

  for (auto &e : entities_rotcmp)
  {
    RotationComponent *rotcmp = e->GetComponent<RotationComponent>();
    Base::TransformComponent *transcmp = e->GetComponent<Base::TransformComponent>();

    rotcmp->rotationVelocity = Lerp(                                                              //
      rotcmp->rotationVelocity, rotcmp->targetRotationVelocity, rotcmp->rotationAcceleration * dt //
    );
    transcmp->rotation += rotcmp->rotationVelocity * rotcmp->rotationSpeed * dt;
  }
}
