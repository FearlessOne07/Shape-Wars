#include "Player.hpp"
#include "EntityManager/Entity.hpp"
#include "raylib.h"
#include "raymath.h"

Player::Player(Color color, float speed, float acceleration, Vector2 position)
    : Entity(color, speed, acceleration, position) {
  _canShoot = true;
  _fireRate = 0.3;
  ;
}

void Player::Update(float dt) {
  GetInput();
  UpdateMovement(dt);
}

void Player::Render() { DrawPolyLinesEx(_position, 8, _radius, 0, 4, _color); }

void Player::GetInput() {

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
    Shoot();
  }
}

void Player::UpdateMovement(float dt) {

  if (Vector2Length(_targetVelocity) > 0) {
    _targetVelocity = Vector2Normalize(_targetVelocity);
  }
  _velocity = Vector2Lerp(_velocity, _targetVelocity, _acceleration * dt);
  _position = Vector2Add(_position, Vector2Scale(_velocity, _speed * dt));
}

bool Player::IsColliding(Entity &other) { return 0; }
