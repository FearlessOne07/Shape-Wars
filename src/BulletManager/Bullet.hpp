#pragma once
#include "Core/Game/RenderContext.hpp"
#include "Utils/Damage.hpp"
#include "raylib.h"

class Entity;
class Bullet {
protected:
  Vector2 _position = {0.f};
  Vector2 _velocity = {0.f};
  float _speed = 0.f;
  float _radius = 0.f;

  Damage _damage;

  bool _isAlive = true;

  const Entity *_source;

public:
  Bullet(Vector2 position, float speed, Damage damage, const Entity *source)
      : _position(position), _speed(speed), _velocity({0}), _radius(10),
        _damage(damage), _isAlive(true), _source(source) {}

  virtual ~Bullet() {}
  virtual void Update(float dt, const RenderContext &renderContext) = 0;
  virtual void Render() = 0;

  void CheckActivity(const RenderContext &renderContext) {
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

  // Access
  Vector2 GetPosition() const { return _position; }
  float GetRadius() const { return _radius; }
  const Entity *GetSource() const { return _source; }

  // Mutation
  void SetIsAlive(bool alive) { _isAlive = alive; }

  // State
  bool IsAlive() const { return _isAlive; }

  const Damage &GetDamage() {
    _damage.direction = _velocity;
    return _damage;
  }
};
