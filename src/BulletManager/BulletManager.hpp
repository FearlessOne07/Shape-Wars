#pragma once
#include "Bullet.hpp"
#include <memory>
#include <vector>
class BulletManager
{
private:
  std::vector<std::unique_ptr<Bullet>> _bullets;

  void RemoveDeadBullets();

public:
  // Core
  void Init();
  void Reset();
  void Update(float dt, const RenderContext &renderContext);
  void Render();

  // Bullets
  void SpawnBullet(std::unique_ptr<Bullet> &bullet);

  const std::vector<std::unique_ptr<Bullet>> &GetBullets() const;
};
