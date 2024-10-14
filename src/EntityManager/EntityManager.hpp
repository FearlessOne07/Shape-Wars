#pragma once

#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "EntityManager/WaveSpecification.hpp"
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
  std::vector<std::unique_ptr<Entity>> _entities;
  std::function<void(std::unique_ptr<Bullet> &)> _bulletSpawnCallback;
  GetBulletsCallBack _getBulletsCallback;
  std::unique_ptr<Entity> _player;

  // Config
  std::unordered_map<std::string, Json::Value> _entityConfigs;

private: // Methods
  bool ValidatePosition(Vector2 position);
  void CheckBulletCollisions();
  void RemoveDeadEntities();
  void LoadConfigs(const std::filesystem::path &path);
  EntitySpec SpecFromJson(const Json::Value &json);

public:
  // Core
  void Init();
  void Update(float dt, const RenderContext &rendercontext);
  void Render();
  void Reset();

  // Entities
  void SpawnPlayer(Color color, float speed, int accelerationFactor,
                   Vector2 startPos);
  void AddEntity(std::unique_ptr<Entity> &entity);
  void SpawnWave(const WaveSpecification &waveSpec,
                 const RenderContext &rendercontext);

  // Callbacks
  void SetBulletSpawnCallBack(
      std::function<void(std::unique_ptr<Bullet> &)> bulletSpawnCallback);
  void SetGetBulletsCallBack(GetBulletsCallBack callback);

  // Access
  const Player *GetPlayer() const;
};
