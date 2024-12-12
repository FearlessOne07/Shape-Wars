#pragma once
#include "raylib.h"
struct Damage
{
  int hitPoints = 0;
  float force = 0.f;
  Vector2 direction = {0, 0};
};
