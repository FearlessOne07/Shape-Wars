#include "EntityManager/EntityManager.hpp"
#include "BulletManager/Bullet.hpp"
#include "BulletManager/PlayerBullet/PlayerBullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "Enemies/Chaser/Chaser.hpp"
#include "EntityManager/WaveSpecification.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <memory>
#include <random>
#include <utility>
#include <vector>

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
  CheckBulletCollisions();
  RemoveDeadEntities();
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

void EntityManager::SpawnWave(const WaveSpecification &waveSpec,
                              const RenderContext &rendercontext) {

  std::random_device rd;
  std::mt19937_64 gen(rd());

  std::uniform_real_distribution<float> angleDist(0, 2 * PI);

  float spawnRadiusMin = 400 / rendercontext.scale;
  float spawnRadiusMax = 1000 / rendercontext.scale;
  std::uniform_real_distribution<float> radiusDist(0.0f, 1.0f);

  std::uniform_int_distribution<int> poolDist(0, waveSpec.pool.size() - 1);

  _entities.reserve(_entities.size() + waveSpec.count);

  for (int i = 0; i < waveSpec.count; i++) {

    bool validPosition = false;
    float angle = angleDist(gen);
    float radius =
        std::sqrt(radiusDist(gen)) * (spawnRadiusMax - spawnRadiusMin) +
        spawnRadiusMin;

    float positionX = sin(angle) * radius;
    float positionY = cos(angle) * radius;
    validPosition = ValidatePosition(Vector2{positionX, positionY});
    for (int a = 0; a < 10 && !validPosition; a++) {
      angle = angleDist(gen);
      positionX = sin(angle) * radius;
      positionY = cos(angle) * radius;
      validPosition = ValidatePosition(Vector2{positionX, positionY});
    }

    EnemyName type = waveSpec.pool[poolDist(gen)];

    switch (type) {
    case EnemyName::CHASER: {
      auto enemy =
          std::make_unique<Chaser>(RED, 90, 2, Vector2{positionX, positionY});
      enemy->SetGetPlayerCallBack([this]() { return this->GetPlayer(); });
      _entities.emplace_back(std::move(enemy));
      break;
    }
    }
  }
}

bool EntityManager::ValidatePosition(Vector2 position) {
  for (auto &e : _entities) {
    if (Vector2Distance(position, e->GetPosition()) < 100) {
      return false;
    }
  }
  return true;
}

void EntityManager::CheckBulletCollisions() {

  for (auto &b : _getBulletsCallback()) {

    PlayerBullet *playerBullet = nullptr;
    if ((playerBullet = dynamic_cast<PlayerBullet *>(b.get()))) {
      for (auto &e : _entities) {
        Vector2 playerBulletPos = playerBullet->GetPosition();
        float playerBulletRad = playerBullet->GetRadius();

        if (CheckCollisionCircles(playerBulletPos, playerBulletRad,
                                  e->GetPosition(), e->GetRadius())) {
          e->SetIsAlive(false);
          playerBullet->SetIsAlive(false);
        }
      }
    }
  }
}

void EntityManager::SetGetBulletsCallBack(GetBulletsCallBack callBack) {
  _getBulletsCallback = callBack;
}

void EntityManager::RemoveDeadEntities() {

  auto it =
      std::remove_if(_entities.begin(), _entities.end(),
                     [](std::unique_ptr<Entity> &e) { return !e->IsAlive(); });
  _entities.erase(it, _entities.end());
}
