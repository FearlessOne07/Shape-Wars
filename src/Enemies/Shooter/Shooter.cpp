#include "Shooter.hpp"
#include "BulletManager/PlayerBullet/PlayerBullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include "raymath.h"
#include <memory>

Shooter::Shooter(EntitySpec entitySpec) : Entity(entitySpec) { _color = GREEN; }

// Overrides
void Shooter::Update(float dt, const RenderContext &renderContext) {
  if (!_hasPlanted) {
    ApproachPlayer(dt);
  } else {
    Attack(dt);
  }
  UpdateMovement(dt);
  CheckActivity();
}
void Shooter::Render() {
  DrawPolyLinesEx(_position, 5, _radius, _rotation, 5, _color);
}

// Core
void Shooter::Attack(float dt) {
  _fireTimer += dt;
  _targetRotation = 2;

  if (_fireTimer >= _fireRate) {
    std::unique_ptr<Bullet> bullet = std::make_unique<PlayerBullet>(
        _position, 1000.f, _damage, _getPlayerCallBack()->GetPosition());
    _bulletSpawnCallback(bullet);
    _fireTimer = 0.f;
  }
}

void Shooter::ApproachPlayer(float dt) {
  Vector2 playerPosition = _getPlayerCallBack()->GetPosition();
  _targetVelocity = Vector2Subtract(playerPosition, _position);
  _targetVelocity = Vector2Normalize(_targetVelocity);
  float distanceFromPlayer =
      Vector2Distance(_position, _getPlayerCallBack()->GetPosition());

  if (distanceFromPlayer <= _plantDistance) {
    _targetVelocity = {0, 0};
    _hasPlanted = true;
  }
}
