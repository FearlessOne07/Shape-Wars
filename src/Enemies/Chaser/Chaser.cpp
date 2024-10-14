#include "Chaser.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

Chaser::Chaser(EntitySpec entitySpec) : Entity(entitySpec) { _color = RED; }

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
  Rotate(dt);
  ChasePlayer();
  UpdateMovement(dt);
  CheckActivity();
}

void Chaser::Rotate(float dt) { _rotation += dt * _rotationSpeed; }

void Chaser::Render() {
  std::cout << "radius; " << _radius << "\n";
  DrawPolyLinesEx(_position, 3, _radius, _rotation, 5, _color);
}

void Chaser::CheckActivity() {
  if (_healthPoints <= 0) {
    _isAlve = false;
  }
}
