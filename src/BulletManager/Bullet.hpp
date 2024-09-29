#pragma once
#include "raylib.h"

class Bullet {
protected:
  Vector2 _position;
  Vector2 _velocity;
  float _speed;
  float _radius;

public:
  Bullet(Vector2 position, float speed)
      : _position(position), _speed(speed), _velocity({0}), _radius(10) {}

  virtual ~Bullet() {}
  virtual void Update(float dt) = 0;
  virtual void Render() = 0;
};
