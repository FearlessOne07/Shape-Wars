#pragma once
#include "raylib.h"
struct EntitySpec {
  // Read-in
  float radius;
  float speed;
  float acceleration;
  float rotationSpeed;
  int healthPoints;
  bool canShoot;
  float fireRate;
  float damage;

  // Determined By eternal body
  Vector2 position;
};
