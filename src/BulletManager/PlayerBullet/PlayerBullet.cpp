#include "PlayerBullet.hpp"
#include "raylib.h"
#include "raymath.h"

PlayerBullet::PlayerBullet(Vector2 position, float speed, Vector2 target)
    : Bullet(position, speed), _target(target) {
  _velocity = Vector2Subtract(_target, _position);
  _velocity = Vector2Normalize(_velocity);
}

void PlayerBullet::Update(float dt, const RenderContext &renderContext) {
  UpdateMovment(dt);
  CheckActivity(renderContext);
}

void PlayerBullet::Render() {
  DrawPolyLinesEx(_position, 6, _radius, 0, 3, RAYWHITE);
}

void PlayerBullet::UpdateMovment(float dt) {
  _position = Vector2Add(Vector2Scale(_velocity, _speed * dt), _position);
}

void PlayerBullet::CheckActivity(const RenderContext &renderContext) {
  float minWidth = (0 - renderContext.marginX) / renderContext.scale;
  float maxWidth =
      (GetScreenWidth() - renderContext.marginX) / renderContext.scale;

  float minHeight = (0 - renderContext.marginX) / renderContext.scale;
  float maxHeight =
      (GetScreenHeight() - renderContext.marginY) / renderContext.scale;

  Vector2 minBounds =
      GetScreenToWorld2D({minWidth, minHeight}, renderContext.camera);
  Vector2 maxBounds =
      GetScreenToWorld2D({maxWidth, maxHeight}, renderContext.camera);

  if (_position.x < minBounds.x || _position.x > maxBounds.x ||
      _position.y < minBounds.y || _position.y > maxBounds.y) {
    _isAlive = false;
  }
}
