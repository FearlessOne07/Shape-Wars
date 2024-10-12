#pragma once
#include "Core/Game/RenderContext.hpp"
#include "raylib.h"

class Bullet {
protected:
  Vector2 _position;
  Vector2 _velocity;
  float _speed;
  float _radius;

  bool _isAlive;

public:
  Bullet(Vector2 position, float speed)
      : _position(position), _speed(speed), _velocity({0}), _radius(10),
        _isAlive(true) {}

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
};
