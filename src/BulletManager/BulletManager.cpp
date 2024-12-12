#include "BulletManager.hpp"
#include "Bullet.hpp"
#include <algorithm>
#include <utility>

void BulletManager::Init()
{
  _bullets.reserve(20);
}
void BulletManager::Update(float dt, const RenderContext &renderContext)
{
  for (auto &b : _bullets)
  {
    b->Update(dt, renderContext);
  }
  RemoveDeadBullets();
}

void BulletManager::Render()
{
  for (auto &b : _bullets)
  {
    b->Render();
  }
}

void BulletManager::SpawnBullet(std::unique_ptr<Bullet> &bullet)
{
  _bullets.emplace_back(std::move(bullet));
}

void BulletManager::RemoveDeadBullets()
{
  auto it = std::remove_if(_bullets.begin(), _bullets.end(), [](std::unique_ptr<Bullet> &b) { return !b->IsAlive(); });
  _bullets.erase(it, _bullets.end());
}

const std::vector<std::unique_ptr<Bullet>> &BulletManager::GetBullets() const
{
  return _bullets;
}
void BulletManager::Reset()
{
  _bullets.clear();
}
