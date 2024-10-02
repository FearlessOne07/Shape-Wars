#include "EntityManager/EntityManager.hpp"
#include "Player/Player.hpp"

void EntityManager::Init() {
  _entities = std::vector<std::unique_ptr<Entity>>();
  _entities.reserve(30);
  _bulletSpawnCallback = nullptr;
  _player = nullptr;
}

void EntityManager::Update(float dt, const RenderContext &rendercontext) {
  _player->Update(dt, rendercontext);
  for (auto &e : _entities) {
    e->Update(dt, rendercontext);
  }
}

void EntityManager::Render() {
  _player->Render();
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
  entity->SetGetPlayerCallBack([this]() { return this->GetPlayer(); });
  _entities.emplace_back(std::move(entity));
}

const Player *EntityManager::GetPlayer() const {
  if (_player) {
    return static_cast<Player *>(_player.get());
  }
  return nullptr;
}

void EntityManager::SpawnPlayer(Color color, float speed,
                                int accelerationFactor, Vector2 startPos) {
  if (!_player) {
    _player =
        std::make_unique<Player>(color, speed, accelerationFactor, startPos);
    _player->SetBulletSpawnCallback(_bulletSpawnCallback);
  }
}

void EntityManager::Reset() {
  _entities.clear();
  _player = nullptr;
}
