#include "Chaser.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include "raymath.h"

Chaser::Chaser(Color color, float speed, int acceleration, Vector2 position)
    : Entity(color, speed, acceleration, position) {}

void Chaser::ChasePlayer() {
  Vector2 direction =
      Vector2Subtract(_getPlayerCallBack()->GetPosition(), _position);

  _targetVelocity = Vector2Normalize(direction);
}

void Chaser::UpdateMovement(float dt) {
  if (Vector2Length(_targetVelocity) > 0) {
    _targetVelocity = Vector2Normalize(_targetVelocity);
  }
  _velocity = Vector2Lerp(_velocity, _targetVelocity, _acceleration * dt);
  _position = Vector2Add(_position, Vector2Scale(_velocity, _speed * dt));
}

void Chaser::Update(float dt, const RenderContext &rendercontext) {
  ChasePlayer();
  UpdateMovement(dt);
}

void Chaser::Rotate(float dt) {}
void Chaser::Render() { DrawPolyLinesEx(_position, 3, _radius, 0, 3, _color); }
bool Chaser::IsColliding(const Entity &other) { return false; }
