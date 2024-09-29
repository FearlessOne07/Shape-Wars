#include "BulletManager.hpp"
#include "Bullet.hpp"
#include <utility>

void BulletManager::Update(float dt) {
  for (auto &b : _bullets) {
    b->Update(dt);
  }
}

void BulletManager::Render() {
  for (auto &b : _bullets) {
    b->Render();
  }
}

void BulletManager::SpawnBullet(std::unique_ptr<Bullet> &bullet) {
  _bullets.push_back(std::unique_ptr<Bullet>(std::move(bullet)));
}
void BulletManager::Reset() { _bullets.clear(); }
