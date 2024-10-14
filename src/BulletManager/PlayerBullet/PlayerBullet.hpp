#pragma once
#include "BulletManager/Bullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "raylib.h"

class PlayerBullet : public Bullet {
private:
  Vector2 _target;

private: // Methods
  void UpdateMovment(float dt);
  void CheckActivity(const RenderContext &renderContext);

public:
  PlayerBullet();

  PlayerBullet(Vector2 position, float speed, float damage, Vector2 target);
  void Update(float dt, const RenderContext &renderContext) override;
  void Render() override;
};
