#pragma once

#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "WaveSpawner/WaveSpawner.hpp"
#include "raylib.h"
#include "json/value.h"
#include <filesystem>
#include <functional>
#include <memory>
#include <vector>

class EntityManager {

  using GetBulletsCallBack =
      std::function<const std::vector<std::unique_ptr<Bullet>> &()>;

private:
  std::vector<std::unique_ptr<Entity>> _entities = {};
  std::function<void(std::unique_ptr<Bullet> &)> _bulletSpawnCallback;
  GetBulletsCallBack _getBulletsCallback;
  std::unique_ptr<Entity> _player;
  EntitySpec _playerSpec = {};

  // Config
  std::unordered_map<int, Json::Value> _entityConfigs = {};
  std::unordered_map<int, EntitySpec> _entitySpecs = {};

  // Utitlity Classes
  WaveSpawner _waveSpawner = {};

  // Spawning
  std::vector<int> _entitiesToSpawn;
  float _entitySpawnInterval = 5;
  float _entitySpawnTimer = _entitySpawnInterval;

  float _waveInterval = 5;
  float _timeSinceLastWave = _waveInterval;

private: // Methods
  // Utitliy
  bool ValidatePosition(Vector2 position);
  void LoadConfigs(const std::filesystem::path &path);
  EntitySpec SpecFromJson(const Json::Value &json);
  void AddEntity(std::unique_ptr<Entity> &entity);

  // Core
  void CheckBulletCollisions();
  void RemoveDeadEntities();
  void CheckPlayerCollisions();

  // Entities
  void SpawnWave(const RenderContext &rendercontext, float dt);

public:
  // Core
  void Init();
  void Update(float dt, const RenderContext &rendercontext);
  void Render();
  void Reset();

  // Callbacks
  void SetBulletSpawnCallBack(
      std::function<void(std::unique_ptr<Bullet> &)> bulletSpawnCallback);
  void SetGetBulletsCallBack(GetBulletsCallBack callback);
  void SpawnPlayer();

  // Access
  const Player *GetPlayer() const;
  int GetAliveCount() const;
};
