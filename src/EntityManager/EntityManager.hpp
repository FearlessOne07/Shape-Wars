#pragma once

#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include <functional>
#include <memory>
#include <vector>

class EntityManager {
private:
  std::vector<std::unique_ptr<Entity>> _entities;
  std::function<void(std::unique_ptr<Bullet> &)> _bulletSpawnCallback;

public:
  void Reset();
  void Update(float dt, const RenderContext &rendercontext);
  void Render();
  void SetBulletSpawnCallBack(
      std::function<void(std::unique_ptr<Bullet> &)> bulletSpawnCallback);
  void AddEntity(std::unique_ptr<Entity> &entity);
};
