#pragma once

#include "EntityManager/EntitySpec.hpp"
#include <unordered_map>
#include <vector>

class WaveSpawner
{
  // Types
  enum struct EnemyTier : int
  {
    EASY = 0,
    MEDIUM,
    HARD
  };

private:
  unsigned int _waveCount = 0;
  unsigned int _wavePoints = 0;
  std::vector<int> _enemiesToSpawn = {};
  std::unordered_map<EnemyTier, float> _tierSpawnChances = {};
  void GenerateWave(const std::unordered_map<int, EntitySpec> &specs);

public:
  WaveSpawner();
  std::vector<int> &SpawnWave(const std::unordered_map<int, EntitySpec> &specs);
  void Reset();
};
