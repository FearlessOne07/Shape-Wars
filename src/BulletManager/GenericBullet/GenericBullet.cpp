#include "GenericBullet.hpp"
#include "EntityManager/Entity.hpp"
#include "Utils/Damage.hpp"
#include "raylib.h"
#include "raymath.h"

GenericBullet::GenericBullet( //
    Vector2 position, float speed, Damage damage, Vector2 target,
    const Entity *source //
    )
    : Bullet(position, speed, damage, source), _target(target) {
  _velocity = Vector2Subtract(_target, _position);
  _velocity = Vector2Normalize(_velocity);
}

void GenericBullet::Update(float dt, const RenderContext &renderContext) {
  UpdateMovment(dt);
  CheckActivity(renderContext);
}

void GenericBullet::Render() {
  DrawPolyLinesEx(_position, 6, _radius, 0, 3, RAYWHITE);
}

void GenericBullet::UpdateMovment(float dt) {
  _position = Vector2Add(Vector2Scale(_velocity, _speed * dt), _position);
}
