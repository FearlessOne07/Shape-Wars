#pragma once
#include "BulletManager/Bullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "Utils/Damage.hpp"
#include "raylib.h"

class GenericBullet : public Bullet
{
private:
  Vector2 _target = {0.f};

private: // Methods
  void UpdateMovment(float dt);

public:
  GenericBullet();

  GenericBullet(Vector2 position, float speed, Damage damage, Vector2 target, const Entity *source);
  void Update(float dt, const RenderContext &renderContext) override;
  void Render() override;
};
