#pragma once
#include <vector>
enum class EnemyName { CHASER = 0 };

struct WaveSpecification {
  int count;
  std::vector<EnemyName> pool;
};
