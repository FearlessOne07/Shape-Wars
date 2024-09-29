#pragma once
#include "Bullet.hpp"
#include <memory>
#include <vector>
class BulletManager {
private:
  std::vector<std::unique_ptr<Bullet>> _bullets;

public:
  BulletManager() = default;
  void Reset();
  void Update(float dt);
  void Render();
  void SpawnBullet(std::unique_ptr<Bullet> &bullet);
};
