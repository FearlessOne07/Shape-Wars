#include "Player.hpp"
#include "BulletManager/GenericBullet/GenericBullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "raylib.h"
#include "raymath.h"

Player::Player(EntitySpec entitySpec) : Entity(entitySpec) {

  _isAlve = true;

  // Rotation
  _targetRotation = 0.5f;
  _rotationAccelertionFactor = 0.5;
  _color = BLUE;

  // Shooting
  _fireTimer = _fireRate;
}

void Player::Update(float dt, const RenderContext &rendercontext) {
  Rotate(dt);
  UpdateTimers(dt);
  GetInput(rendercontext);
  UpdateMovement(dt);
  Wrap(rendercontext);
  CheckActivity();
}

void Player::Render() {
  DrawPolyLinesEx(_position, 6, _radius, _rotation, 3, _color);
}

void Player::GetInput(const RenderContext &rendercontext) {
  if (IsKeyDown(KEY_A)) {
    _targetVelocity.x = -1;
  } else if (IsKeyDown(KEY_D)) {
    _targetVelocity.x = 1;
  } else {
    _targetVelocity.x = 0;
  }

  if (IsKeyDown(KEY_W)) {
    _targetVelocity.y = -1;
  } else if (IsKeyDown(KEY_S)) {
    _targetVelocity.y = 1;
  } else {
    _targetVelocity.y = 0;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    if (_fireTimer >= _fireRate) {

      _fireTimer = 0.f;
      Shoot(rendercontext);
    }
  }
}

void Player::Shoot(const RenderContext &rendercontext) {

  Vector2 mousePosition = GetMousePosition();
  mousePosition = rendercontext.GetScreenToGame(mousePosition);
  mousePosition = GetScreenToWorld2D(mousePosition, rendercontext.camera);
  std::unique_ptr<Bullet> bullet = std::make_unique<GenericBullet>(
      _position, 1000.f, _damage, mousePosition, this);
  _bulletSpawnCallback(bullet);
}

void Player::UpdateTimers(float dt) {
  if (_fireTimer < _fireRate)
    _fireTimer += dt;
}

void Player::Wrap(const RenderContext &rendercontext) {

  Vector2 minBounds = GetScreenToWorld2D({0, 0}, rendercontext.camera);
  Vector2 maxBounds =
      GetScreenToWorld2D({rendercontext.gameWidth, rendercontext.gameHeight},
                         rendercontext.camera);

  if (_position.x >= maxBounds.x + _radius) {
    _position.x = minBounds.x - _radius;
  } else if (_position.x <= minBounds.x - _radius) {
    _position.x = maxBounds.x + _radius;
  }

  if (_position.y >= maxBounds.y + _radius) {
    _position.y = minBounds.y - _radius;
  } else if (_position.y <= minBounds.y - _radius) {
    _position.y = maxBounds.y + _radius;
  }
}
