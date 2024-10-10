#pragma once

#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "EntityManager/WaveSpecification.hpp"
#include <functional>
#include <memory>
#include <vector>

class EntityManager {
private:
  std::vector<std::unique_ptr<Entity>> _entities;
  std::function<void(std::unique_ptr<Bullet> &)> _bulletSpawnCallback;
  std::unique_ptr<Entity> _player;

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
  void SetBulletSpawnCallBack(
      std::function<void(std::unique_ptr<Bullet> &)> bulletSpawnCallback);
  void SpawnWave(const WaveSpecification &waveSpec);

  // Access
  const Player *GetPlayer() const;
};
