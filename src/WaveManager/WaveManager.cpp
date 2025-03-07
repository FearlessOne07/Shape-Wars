#include "WaveManager.hpp"

void WaveManager::Update(float dt, Base::EntityManager *entityManager)
{
  if (_spawnTimer >= _spawnTime)
  {
    _spawnTimer = 0.f;
    SpawnEnemy(entityManager);
  }
  else
  {
    _spawnTimer += dt;
  }
}

void WaveManager::SpawnEnemy(Base::EntityManager *entityManager)
{

}
