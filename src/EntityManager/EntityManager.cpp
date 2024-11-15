#include "EntityManager/EntityManager.hpp"
#include "BulletManager/Bullet.hpp"
#include "BulletManager/GenericBullet/GenericBullet.hpp"
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

  // Load the eneity json configs
  LoadConfigs("config/entities");

  // Initialize entity vector
  _entities = std::vector<std::unique_ptr<Entity>>();
  _entities.reserve(50);
  _bulletSpawnCallback = nullptr;
  _getBulletsCallback = nullptr;
  _player = nullptr;

  _entitiesToSpawn = {};
  _waveSpawner = WaveSpawner();
}

void EntityManager::Update(float dt, const RenderContext &rendercontext) {

  // Update and translate spawn anchors if camera zoom changes
  if (rendercontext.camera.zoom != _lastCameraZoom) {
    GenerateSpawnAnchors(4, rendercontext);
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
  // Render player
  _player->Render();

  // Render enemies
  for (auto &e : _entities) {
    e->Render();
  }
}

void EntityManager::AddEntity(std::unique_ptr<Entity> &entity) {
  // Check if the entity can shoot
  if (entity->CanShoot()) {
    entity->SetBulletSpawnCallback(_bulletSpawnCallback);
  }

  // Set the get player call back
  entity->SetGetPlayerCallBack([this]() { return this->GetPlayer(); });

  // Add the entity
  _entities.emplace_back(std::move(entity));
}

void EntityManager::SpawnPlayer() {
  if (!_player && _bulletSpawnCallback) {
    // Get player spec
    EntitySpec spec = _playerSpec;
    spec.position = {0};

    // Spawn the player
    _player = std::make_unique<Player>(spec);
    _player->SetBulletSpawnCallback(_bulletSpawnCallback);
  } else {
    assert(false);
  }
}

void EntityManager::Reset() {
  _entities.clear();
  _waveSpawner.Reset();
  _player = nullptr;
}

void EntityManager::SpawnWave(const RenderContext &rendercontext, float dt) {

  // Check if there are no enemies and a certain time has passed then spawn
  // wave
  if (_entitiesToSpawn.size() == 0 && _timeSinceLastWave > _waveInterval &&
      _entities.size() == 0) {

    // Get wave contenst from wave spawner
    _entitiesToSpawn = _waveSpawner.SpawnWave(_entitySpecs);
    _timeSinceLastWave = 0;
  } else {
    _timeSinceLastWave += dt;
  }

  // Check if there are enemies to spawn and a certain time has passed after the
  // last enemy
  if (_entitiesToSpawn.size() > 0 &&
      _entitySpawnTimer >= _entitySpawnInterval) {

    // Initialize the random device
    std::random_device rd;
    std::mt19937_64 gen(rd());

    // Select a ranomd spawn anchor
    Vector2 anchor = {0, 0};
    std::uniform_int_distribution<int> anchorDist(0, _spawnAnchors.size() - 1);

    // Select a new anchor if the one chosen is the same as the last chiesen one
    // up to a maximum of five times
    for (int i = 0; i < 5 && (anchor.x == _lastSpawnAnchor.x &&
                              anchor.y == _lastSpawnAnchor.y);
         i++) {
      anchor = _spawnAnchors[anchorDist(gen)];
    }

    // Update the last chosen spawn acnhor
    Vector2 _lastSpawnAnchor = anchor;

    // Get the last enemy in the vector to spawn
    int enemy = _entitiesToSpawn.back();

    // Get it's spec
    EntitySpec spec = _entitySpecs[enemy];
    std::unique_ptr<Entity> enemyToSpawn;

    // set its start posiiton to the acnhor chisen
    spec.position = anchor;

    // Push construct an eneity based on the chosen spec
    if (spec.name == "chaser") {
      enemyToSpawn = std::make_unique<Chaser>(spec);
    } else if (spec.name == "shooter") {
      enemyToSpawn = std::make_unique<Shooter>(spec);
    }

    std::cout << "Spawned " << spec.name << "\n";

    //
    AddEntity(enemyToSpawn);
    _entitiesToSpawn.pop_back();
    _entitySpawnTimer = 0;
  } else {
    _entitySpawnTimer += dt;
  }
}

void EntityManager::CheckBulletCollisions() {
  for (auto &b : _getBulletsCallback()) {
    GenericBullet *playerBullet = nullptr;
    if ((playerBullet = dynamic_cast<GenericBullet *>(b.get())) &&
        b->GetSource() == _player.get()) {
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
    } else if ((playerBullet = dynamic_cast<GenericBullet *>(b.get())) &&
               b->GetSource() != _player.get()) {

      if (CheckCollisionCircles(_player->GetPosition(), _player->GetRadius(),
                                b->GetPosition(), b->GetRadius()) &&
          _player->IsAlive()) {
        _player->SetHp(_player->GetHp() - b->GetDamage());
        b->SetIsAlive(false);
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
    spec.tier = json["tier"].asInt();
    spec.unlockWave = json["unlock-wave"].asInt();
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
  assert(_player);
  return static_cast<Player *>(_player.get());
}
