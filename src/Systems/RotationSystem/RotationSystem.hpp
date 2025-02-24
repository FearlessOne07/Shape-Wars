#pragma once
#include "base/System.hpp"

class RotationSystem : public Base::System
{
public:
  virtual void Update(float dt, Base::EntityManager *entityManager) override;
};
