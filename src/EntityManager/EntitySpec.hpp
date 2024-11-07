#pragma once
#include "raylib.h"
#include <string>
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
  bool canClump;

  // Determined By eternal body
  Vector2 position;

  // Not Read By entity
  int cost;
  std::string name;
};
