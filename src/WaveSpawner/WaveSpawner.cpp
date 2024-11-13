#include "WaveSpawner.hpp"
#include "EntityManager/EntitySpec.hpp"
#include <cassert>
#include <iostream>
#include <random>

WaveSpawner::WaveSpawner() {
  _tierSpawnChances = {
      {EnemyTier::EASY, 0.6}, {EnemyTier::MEDIUM, 0.3}, {EnemyTier::HARD, 0.1}};
}

std::vector<int> &
WaveSpawner::SpawnWave(const std::unordered_map<int, EntitySpec> &specs) {
  _enemiesToSpawn.clear();
  _enemiesToSpawn.reserve(20);
  GenerateWave(specs);
  return _enemiesToSpawn;
}

// TODO: Refine spawing mechanic
void WaveSpawner::GenerateWave(
    const std::unordered_map<int, EntitySpec> &specs) {

  assert(!specs.empty());
  _wavePoints = 5 * ++_waveCount;

  std::cout << "This is Wave: " << _waveCount << "\n";

  std::random_device rd;
  std::mt19937_64 gen(rd());

  std::vector<int> pool = {};
  for (auto &[enemy, spec] : specs) {
    if (_waveCount >= spec.unlockWave) {
      std::uniform_real_distribution<float> probDist(0.f, 1.f);
      if (probDist(gen) <=
          _tierSpawnChances[static_cast<EnemyTier>(spec.tier)]) {
        pool.push_back(enemy);
      }
    }
  }

  if (pool.empty())
    return;

  std::uniform_int_distribution<int> poolDist(0, pool.size() - 1);
  while (_wavePoints > 0) {
    int enemyType = pool[poolDist(gen)];
    if (specs.at(enemyType).cost <= _wavePoints) {
      _wavePoints -= specs.at(enemyType).cost;
      _enemiesToSpawn.push_back(enemyType);
    }
  }

  std::cout << "Upcoming Wave has " << _enemiesToSpawn.size() << " enemies\n";
}

void WaveSpawner::Reset() {
  _waveCount = 0;
  _wavePoints = 0;
  _enemiesToSpawn.clear();
}
