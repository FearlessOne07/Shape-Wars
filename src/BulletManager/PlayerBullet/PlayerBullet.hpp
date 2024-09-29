#pragma once
#include "BulletManager/Bullet.hpp"
#include "raylib.h"

class PlayerBullet : public Bullet {
private:
  Vector2 _target;

private: // Methods
  void UpdateMovment(float dt);

public:
  PlayerBullet();

  PlayerBullet(Vector2 position, float speed, Vector2 target);
  void Update(float dt) override;
  void Render() override;
};
