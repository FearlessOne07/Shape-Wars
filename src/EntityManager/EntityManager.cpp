#include "EntityManager/EntityManager.hpp"
#include "BulletManager/Bullet.hpp"
#include "BulletManager/PlayerBullet/PlayerBullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "Enemies/Chaser/Chaser.hpp"
#include "Enemies/Shooter/Shooter.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "Player/Player.hpp"
#include "WaveSpawner/WaveSpawner.hpp"
#include "raylib.h"
#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
#include <algorithm>
#include <cassert>
#include <cmath>
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
  _getBulletsCallback = nullptr;
  _player = nullptr;

  _entitiesToSpawn = {};
  _waveSpawner = WaveSpawner();
}

void EntityManager::Update(float dt, const RenderContext &rendercontext) {

  if (!_shiftedSpawnAnchors || rendercontext.camera.zoom != _lastCameraZoom) {
    GenerateSpawnAnchors(4, rendercontext);
    _shiftedSpawnAnchors = true;
    _lastCameraZoom = rendercontext.camera.zoom;
    std::cout << "Shifted spawn anchor\n";
  }

  // Check if there are any enemies to spawn and spawn them
  SpawnWave(rendercontext, dt);

  // Update player
  _player->Update(dt, rendercontext);
  CheckPlayerCollisions();

  // Update enemies
  for (auto &e : _entities) {
    e->Update(dt, rendercontext);
  }
  CheckBulletCollisions();

  // Clean up dead enemies
  RemoveDeadEntities();
}

void EntityManager::Render() {
  _player->Render();
  for (auto &e : _entities) {
    e->Render();
  }

  for (Vector2 &v : _spawnAnchors) {
    DrawCircleV(v, 5, RED);
  }
}

void EntityManager::AddEntity(std::unique_ptr<Entity> &entity) {
  if (entity->CanShoot()) {
    entity->SetBulletSpawnCallback(_bulletSpawnCallback);
  }

  entity->SetGetPlayerCallBack([this]() { return this->GetPlayer(); });
  _entities.emplace_back(std::move(entity));
}

void EntityManager::SpawnPlayer() {
  if (!_player && _bulletSpawnCallback) {
    EntitySpec spec = _playerSpec;
    spec.position = {0};
    _player = std::make_unique<Player>(spec);
    _player->SetBulletSpawnCallback(_bulletSpawnCallback);
  } else {
    assert(false);
  }
}

void EntityManager::Reset() {
  _entities.clear();
  _player = nullptr;
}

void EntityManager::SpawnWave(const RenderContext &rendercontext, float dt) {

  if (_entities.size() == 0 && _timeSinceLastWave > _waveInterval) {
    _entitiesToSpawn = _waveSpawner.SpawnWave(_entitySpecs);
    _timeSinceLastWave = 0;
  } else {
    _timeSinceLastWave += dt;
  }

  if (_entitiesToSpawn.size() > 0 &&
      _entitySpawnTimer >= _entitySpawnInterval) {

    // Initialize the random device
    std::random_device rd;
    std::mt19937_64 gen(rd());

    Vector2 anchor = {0, 0};
    std::uniform_int_distribution<int> anchorDist(0, _spawnAnchors.size() - 1);

    for (int i = 0; i < 5 && (anchor.x == _lastSpawnAnchor.x &&
                              anchor.y == _lastSpawnAnchor.y);
         i++) {
      anchor = _spawnAnchors[anchorDist(gen)];
    }
    Vector2 _lastSpawnAnchor = anchor;

    int enemy = _entitiesToSpawn.back();
    EntitySpec spec = _entitySpecs[enemy];
    std::unique_ptr<Entity> enemyToSpawn;
    spec.position = anchor;
    if (spec.name == "chaser") {
      enemyToSpawn = std::make_unique<Chaser>(spec);
    } else if (spec.name == "shooter") {
      enemyToSpawn = std::make_unique<Shooter>(spec);
    }
    std::cout << "Spawned " << spec.name << "\n";
    enemyToSpawn->SetGetPlayerCallBack([this]() { return this->GetPlayer(); });
    AddEntity(enemyToSpawn);
    _entitiesToSpawn.pop_back();
    _entitySpawnTimer = 0;
  } else {
    _entitySpawnTimer += dt;
  }
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
          _player->SetHp(_player->GetHp() - e->GetDamage());
        }
      }
    }
  }
}

// Utitlity functions
void EntityManager::LoadConfigs(const std::filesystem::path &path) {
  Json::Reader reader;

  int i = 0;
  for (auto &entry : std::filesystem::directory_iterator(path)) {
    if (entry.path().extension() == ".json") {
      std::fstream file(entry.path());
      Json::Value config;

      if (reader.parse(file, config)) {
        if (entry.path().stem().string() != "player") {
          _entitySpecs[i++] = SpecFromJson(config);
        } else if (entry.path().stem().string() == "player") {
          _playerSpec = SpecFromJson(config);
        }
      }
    }
  }
}

EntitySpec EntityManager::SpecFromJson(const Json::Value &json) {
  assert((Json::nullValue != json.type()));

  if (Json::nullValue != json.type()) {

    EntitySpec spec;

    spec.radius = json["radius"].asFloat();
    spec.speed = json["speed"].asFloat();
    spec.acceleration = json["acceleration"].asFloat();
    spec.rotationSpeed = json["rotation-speed"].asFloat();
    spec.healthPoints = json["health-points"].asInt();
    spec.canShoot = json["can-shoot"].asBool();
    spec.fireRate = json["fire-rate"].asFloat();
    spec.damage = json["damage"].asFloat();
    spec.position = {0, 0};

    // Not Read By entity
    spec.cost = json["cost"].asInt();
    spec.name = json["name"].asString();
    std::cout << "Name of current: " << spec.name << "\n";
    return spec;
  }
  return {};
}

void EntityManager::GenerateSpawnAnchors(int countPerSide,
                                         const RenderContext &rendercontext) {

  float offset = 50.f;
  int currentAnchor = 0;

  // Right hand side
  float distanceV = rendercontext.gameHeight / (float)(countPerSide - 1);
  float distanceH = rendercontext.gameWidth / ((float)countPerSide - 1);

  for (int i = 0; i < countPerSide; i++) {
    Vector2 anchor = {rendercontext.gameWidth + offset, (distanceV * i)};
    _spawnAnchors[currentAnchor++] = anchor;
  }

  // For Left
  for (int i = 0; i < countPerSide; i++) {
    Vector2 anchor = {-offset, (distanceV * i)};
    _spawnAnchors[currentAnchor++] = anchor;
  }

  // For Bottom
  for (int i = 0; i < countPerSide; i++) {
    Vector2 anchor = {(distanceH * i), rendercontext.gameHeight + offset};
    _spawnAnchors[currentAnchor++] = anchor;
  }
  // For Top
  for (int i = 0; i < countPerSide; i++) {
    Vector2 anchor = {(distanceH * i), -offset};
    _spawnAnchors[currentAnchor++] = anchor;
  }

  for (Vector2 &v : _spawnAnchors) {
    v.x = v.x - ((rendercontext.gameWidth) / 2);
    v.y = v.y - ((rendercontext.gameHeight) / 2);

    v.x = v.x / rendercontext.camera.zoom;
    v.y = v.y / rendercontext.camera.zoom;
  }
}

// Call Backs
void EntityManager::SetGetBulletsCallBack(GetBulletsCallBack callBack) {
  _getBulletsCallback = callBack;
}
void EntityManager::SetBulletSpawnCallBack(
    SpawnBulletCallBack bulletSpawnCallback) {
  _bulletSpawnCallback = bulletSpawnCallback;
}

// Access And Mutation
int EntityManager::GetAliveCount() const { return _entities.size(); }

const Player *EntityManager::GetPlayer() const {
  if (_player) {
    return static_cast<Player *>(_player.get());
  }
  return nullptr;
}
