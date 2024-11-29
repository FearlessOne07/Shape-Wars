#pragma once
#include "Utils/Damage.hpp"
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
  Damage damage;
  bool canClump;
  int tier;
  int unlockWave;

  // Determined By eternal body
  Vector2 position;

  // Not Read By entity
  int cost;
  std::string name;
};
