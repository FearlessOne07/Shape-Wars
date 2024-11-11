#include "Shooter.hpp"
#include "BulletManager/PlayerBullet/PlayerBullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "Player/Player.hpp"
#include "raymath.h"
#include <memory>

Shooter::Shooter(EntitySpec entitySpec) : Entity(entitySpec) {}

// Overrides
void Shooter::Update(float dt, const RenderContext &renderContext) {}
void Shooter::Render() {}
void Shooter::UpdateMovement(float dt) {}
void Shooter::Rotate(float dt) {}
void Shooter::CheckActivity() {}

// Core
void Shooter::Shoot(float dt) {
  _fireTimer += dt;
  _targetRotation = 2;

  if (_fireTimer >= _fireRate) {
    std::unique_ptr<Bullet> bullet = std::make_unique<PlayerBullet>(
        _position, 1000.f, _damage, _getPlayerCallBack()->GetPosition());
    _bulletSpawnCallback(bullet);
  }
}
void Shooter::ApproachPlayer(float dt) {
  if (!_hasPlanted) {
    Vector2 playerPosition = _getPlayerCallBack()->GetPosition();
    _targetVelocity = Vector2Subtract(playerPosition, _position);
    _targetVelocity = Vector2Normalize(_targetVelocity);
  }
}
