#include "WaveSpawner.hpp"
#include "EntityManager/EntitySpec.hpp"
#include <cassert>
#include <random>

std::vector<int> &
WaveSpawner::SpawnWave(const std::unordered_map<int, EntitySpec> &specs) {

  _enemiesToSpawn.clear();
  _enemiesToSpawn.reserve(10);
  GenerateWave(specs);
  return _enemiesToSpawn;
}

void WaveSpawner::GenerateWave(
    const std::unordered_map<int, EntitySpec> &specs) {

  assert(!specs.empty());
  _waveCount++;
  _wavePoints = 10 * _waveCount;

  std::vector<int> pool = {};

  for (auto [enemy, spec] : specs) {
    pool.push_back(enemy);
  }

  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<int> poolDist(0, pool.size() - 1);

  while (_wavePoints > 0) {
    int enemyType = poolDist(gen);
    if (specs.at(enemyType).cost <= _wavePoints) {
      _wavePoints -= specs.at(enemyType).cost;
      pool.erase(pool.begin() + enemyType);
      _enemiesToSpawn.push_back(enemyType);
    }
  }
}