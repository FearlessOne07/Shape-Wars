#pragma once
#include "base/Component.hpp"
#include "raylib.h"

struct ShootComponent : public Base::Component
{
  Vector2 target = {0, 0};
  float bulletSpeed = 0.f;
  float bulletFireRate = 0.f;
  float bulletFireTimer = 0.f;
  bool IsFiring = false;
};
