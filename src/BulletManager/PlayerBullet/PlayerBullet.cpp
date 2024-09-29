#include "PlayerBullet.hpp"
#include "raylib.h"
#include "raymath.h"

PlayerBullet::PlayerBullet(Vector2 position, float speed, Vector2 target)
    : Bullet(position, speed), _target(target) {
  _velocity = Vector2Subtract(_target, _position);
  _velocity = Vector2Normalize(_velocity);
}

void PlayerBullet::Update(float dt) { UpdateMovment(dt); }

void PlayerBullet::Render() { DrawCircleLinesV(_position, _radius, RAYWHITE); }

void PlayerBullet::UpdateMovment(float dt) {
  _position = Vector2Add(_velocity, _position);
}
