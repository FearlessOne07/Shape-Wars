#include "EntityManager/EntityManager.hpp"
#include "BulletManager/Bullet.hpp"
#include "BulletManager/PlayerBullet/PlayerBullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "Enemies/Chaser/Chaser.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "EntityManager/WaveSpecification.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include "raymath.h"
#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <utility>
#include <vector>

void EntityManager::Init() {
  LoadConfigs("config/entities");
  _entities = std::vector<std::unique_ptr<Entity>>();
  _entities.reserve(50);
  _bulletSpawnCallback = nullptr;
  _player = nullptr;
}

void EntityManager::Update(float dt, const RenderContext &rendercontext) {

  if (_player && _player->IsAlive()) {
    _player->Update(dt, rendercontext);
  }

  for (auto &e : _entities) {
    e->Update(dt, rendercontext);
  }

  CheckBulletCollisions();
  RemoveDeadEntities();
}

void EntityManager::Render() {

  if (_player && _player->IsAlive()) {
    _player->Render();
  }

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
    EntitySpec spec = SpecFromJson(_entityConfigs["player"]);
    spec.position = {0};
    _player = std::make_unique<Player>(spec);
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
    EntitySpec spec;

    switch (type) {
    case EnemyName::CHASER: {
      spec = SpecFromJson(_entityConfigs["chaser"]);
      spec.position = {positionX, positionY};

      auto enemy = std::make_unique<Chaser>(spec);
      enemy->SetGetPlayerCallBack([this]() { return this->GetPlayer(); });

      _entities.emplace_back(std::move(enemy));
      break;
    }
    }

    std::cout << "Spawned Enemy " << i << "\n";
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
                                  e->GetPosition(), e->GetRadius()) &&
            b->IsAlive()) {
          e->SetHp(e->GetHp() - playerBullet->GetDamage());
          playerBullet->SetIsAlive(false);
        }
      }
    }
  }
}

void EntityManager::RemoveDeadEntities() {
  auto it =
      std::remove_if(_entities.begin(), _entities.end(),
                     [](std::unique_ptr<Entity> &e) { return !e->IsAlive(); });
  _entities.erase(it, _entities.end());
}

void EntityManager::CheckPlayerCollisions() {

  if (_player && _player->IsAlive()) {
    for (auto &e : _entities) {
      if (e->IsAlive()) {

        Vector2 playerPos = _player->GetPosition();
        float playerRadius = _player->GetRadius();

        if (CheckCollisionCircles(playerPos, playerRadius, e->GetPosition(),
                                  e->GetRadius())) {
        }
      }
    }
  }
}

void EntityManager::SetGetBulletsCallBack(GetBulletsCallBack callBack) {
  _getBulletsCallback = callBack;
}

void EntityManager::LoadConfigs(const std::filesystem::path &path) {
  Json::Reader reader;

  for (auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.path().extension() == ".json") {
      std::fstream file(entry.path());
      Json::Value config;

      if (reader.parse(file, config)) {
        _entityConfigs[entry.path().stem().string()] = config;
      }
    }
  }
}

EntitySpec EntityManager::SpecFromJson(const Json::Value &json) {
  assert((Json::nullValue != json.type()));
  if (Json::nullValue != json.type()) {
    return {
        json["radius"].asFloat(),       json["speed"].asFloat(),
        json["acceleration"].asFloat(), json["rotation-speed"].asFloat(),
        json["health-points"].asInt(),  json["can-shoot"].asBool(),
        json["fire-rate"].asFloat(),
    };
  }
  return {};
}
