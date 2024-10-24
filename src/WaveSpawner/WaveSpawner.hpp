#pragma once

#include "EntityManager/EntitySpec.hpp"
#include <unordered_map>
#include <vector>

class WaveSpawner {
private:
  unsigned int _waveCount = 0;
  unsigned int _wavePoints = 0;
  std::vector<int> _enemiesToSpawn = {};
  void GenerateWave(const std::unordered_map<int, EntitySpec> &specs);

public:
  std::vector<int> &SpawnWave(const std::unordered_map<int, EntitySpec> &specs);
};
