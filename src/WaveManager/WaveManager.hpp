#pragma once

#include "base/EntityManager.hpp"
class WaveManager
{
private:
  float _spawnTimer = 0.f;
  float _spawnTime = 3.f;
  void SpawnEnemy(Base::EntityManager *entityManager);

public:
  void Update(float dt, Base::EntityManager *entityManager);
};
