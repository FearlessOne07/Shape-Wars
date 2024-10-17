#pragma once
#include "Core/Game/RenderContext.hpp"
#include "raylib.h"

class Bullet {
protected:
  Vector2 _position = {0.f};
  Vector2 _velocity = {0.f};
  float _speed = 0.f;
  float _radius = 0.f;

  float _damage = 0.f;

  bool _isAlive = true;

public:
  Bullet(Vector2 position, float speed, float damage)
      : _position(position), _speed(speed), _velocity({0}), _radius(10),
        _damage(damage), _isAlive(true) {}

  virtual ~Bullet() {}
  virtual void Update(float dt, const RenderContext &renderContext) = 0;
  virtual void Render() = 0;

  // Access
  Vector2 GetPosition() const { return _position; }
  float GetRadius() const { return _radius; }

  // Mutation
  void SetIsAlive(bool alive) { _isAlive = alive; }

  // State
  bool IsAlive() { return _isAlive; }

  float GetDamage() const { return _damage; }
};
