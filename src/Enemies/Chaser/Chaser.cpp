#include "Chaser.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include "raymath.h"

Chaser::Chaser(EntitySpec entitySpec) : Entity(entitySpec) { _color = RED; }

void Chaser::ChasePlayer() {
  Vector2 direction =
      Vector2Subtract(_getPlayerCallBack()->GetPosition(), _position);

  _targetVelocity = Vector2Normalize(direction);
}

void Chaser::Update(float dt, const RenderContext &rendercontext) {
  Rotate(dt);
  ChasePlayer();
  UpdateMovement(dt);
  CheckActivity();
}

void Chaser::Render() {
  DrawPolyLinesEx(_position, 3, _radius, _rotation, 5, _color);
}
