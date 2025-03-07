#pragma once
#include "base/Component.hpp"

struct RotationComponent : public Base::Component
{
  float rotationVelocity = 0.f;
  float targetRotationVelocity = 0.f;
  float rotationAcceleration = 0.f;
  float rotationSpeed = 0.f;
};
