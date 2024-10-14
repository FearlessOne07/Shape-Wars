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

  // Determined By eternal body
  Vector2 position;

  operator bool() {
    return radius && speed && acceleration && rotationSpeed && healthPoints &&
           fireRate && ((!fireRate && !canShoot) || (fireRate && canShoot));
  }
};
