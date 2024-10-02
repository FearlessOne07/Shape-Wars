#include "Player.hpp"
#include "BulletManager/PlayerBullet/PlayerBullet.hpp"
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "raylib.h"
#include "raymath.h"

Player::Player(Color color, float speed, int acceleration, Vector2 position)
    : Entity(color, speed, acceleration, position) {
  _canShoot = true;
  _fireRate = 0.3;
  _fireTimer = 0.f;
}

void Player::Update(float dt, const RenderContext &rendercontext) {
  UpdateTimers(dt);
  GetInput(rendercontext);
  UpdateMovement(dt);
}

void Player::Render() { DrawPolyLinesEx(_position, 8, _radius, 0, 4, _color); }

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

void Player::UpdateMovement(float dt) {

  if (Vector2Length(_targetVelocity) > 0) {
    _targetVelocity = Vector2Normalize(_targetVelocity);
  }
  _velocity = Vector2Lerp(_velocity, _targetVelocity, _acceleration * dt);
  _position = Vector2Add(_position, Vector2Scale(_velocity, _speed * dt));
}

bool Player::IsColliding(const Entity &other) { return 0; }

void Player::Shoot(const RenderContext &rendercontext) {
  Vector2 mousePosition = GetMousePosition();

  mousePosition.x =
      (mousePosition.x - rendercontext.marginX) / rendercontext.scale;
  mousePosition.y =
      (mousePosition.y - rendercontext.marginY) / rendercontext.scale;

  std::unique_ptr<Bullet> bullet =
      std::make_unique<PlayerBullet>(_position, 1000.f, mousePosition);
  _bulletSpawnCallback(bullet);
}

void Player::UpdateTimers(float dt) {
  if (_fireTimer < _fireRate)
    _fireTimer += dt;
}
