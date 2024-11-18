#include "WaveSpawner.hpp"
#include "EntityManager/EntitySpec.hpp"
#include <cassert>
#include <iostream>
#include <random>

WaveSpawner::WaveSpawner() {
  _tierSpawnChances = {
      {EnemyTier::EASY, 0.4}, {EnemyTier::MEDIUM, 0.4}, {EnemyTier::HARD, 0.2}};
}

std::vector<int> &
WaveSpawner::SpawnWave(const std::unordered_map<int, EntitySpec> &specs) {
  _enemiesToSpawn.clear();
  _enemiesToSpawn.reserve(20);
  GenerateWave(specs);
  return _enemiesToSpawn;
}

// TODO: Refine spawing mechanic
// TODO 2: REALLY FIX THIS SPAWING SYSTEM
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
      for (int i = 0; i < 3; ++i) { // Retry up to 3 times
        std::uniform_real_distribution<float> probDist(0.f, 1.f);
        float tier = probDist(gen);
        if (tier <= _tierSpawnChances[static_cast<EnemyTier>(spec.tier)]) {
          pool.push_back(enemy);
          break;
        }
      }
    }
  }
  if (pool.empty())
    return;

  std::uniform_int_distribution<int> poolDist(0, pool.size() - 1);
  std::unordered_map<std::string, int> spawned = {};
  while (_wavePoints > 0) {
    int enemyType = pool[poolDist(gen)];
    if (specs.at(enemyType).cost <= _wavePoints) {
      _wavePoints -= specs.at(enemyType).cost;
      if (spawned.find(specs.at(enemyType).name) == spawned.end()) {
        spawned.insert({specs.at(enemyType).name, 1});
      } else {
        ++spawned.at(specs.at(enemyType).name);
      }
      _enemiesToSpawn.push_back(enemyType);
    }
  }

  std::cout << "Upcoming Wave has " << _enemiesToSpawn.size() << " enemies\n";
  for (auto &[name, count] : spawned) {
    std::cout << name << ": " << count << "\n";
  }
}

void WaveSpawner::Reset() {
  _waveCount = 0;
  _wavePoints = 0;
  _enemiesToSpawn.clear();
}
