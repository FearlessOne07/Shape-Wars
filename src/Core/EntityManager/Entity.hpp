#pragma once

#include "raylib.h"
class Entity {
protected: // Attributes
  Color _color;
  float _radius;
  Vector2 _velocity;
  float _speed;
  float _acceleration;
  Vector2 _targetVelocity;
  Vector2 _position;

protected: // Methods
public:
  Entity(Color color, float speed, float acceleration, Vector2 position)
      : _color(color), _velocity({0}), _speed(speed),
        _acceleration(acceleration), _position(position), _targetVelocity({0}),
        _radius(30) {}
  virtual ~Entity() {}
  virtual void Update(float dt) = 0;
  virtual void Render() = 0;
};
