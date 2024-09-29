#include "EntityManager/EntityManager.hpp"

void EntityManager::Update(float dt) {

  for (auto &e : _entities) {
    e->Update(dt);
  }
}

void EntityManager::Render() {

  for (auto &e : _entities) {
    e->Render();
  }
}
void EntityManager::SetBulletSpawnCallBack(
    std::function<void(std::unique_ptr<Bullet> &)> bulletSpawnCallback) {
  _bulletSpawnCallback = bulletSpawnCallback;
}

void EntityManager::AddEntity(std::unique_ptr<Entity> &entity) {
  if (entity->CanShoot()) {
    entity->SetBulletSpawnCallback(_bulletSpawnCallback);
  }
  _entities.push_back(std::unique_ptr<Entity>(std::move(entity)));
}

void EntityManager::Reset() { _entities.clear(); }
