#pragma once
#include <functional>
#include <memory>

#include "raylib.h"
class Bullet;
class Entity {
protected: // Attributes
  Color _color;
  float _radius;
  Vector2 _velocity;
  float _speed;
  float _acceleration;
  Vector2 _targetVelocity;
  Vector2 _position;

  // Shooting
  bool _canShoot;
  float _fireRate;
  float _fireTimer;
  std::function<void(std::unique_ptr<Bullet> &)> _bulletSpawnCallback;

  // State
  bool _isAlve;

protected: // Methods
public:
  Entity(Color color, float speed, float acceleration, Vector2 position)
      : _color(color), _velocity({0}), _speed(speed),
        _acceleration(acceleration), _position(position), _targetVelocity({0}),
        _radius(30), _isAlve(true) {}
  virtual ~Entity() {}
  virtual void Update(float dt, const RenderContext &rendercontext) = 0;
  virtual void Render() = 0;
  virtual bool IsColliding(Entity &other) = 0;
  void SetBulletSpawnCallback(
      std::function<void(std::unique_ptr<Bullet> &)> bulletCallBack) {
    _bulletSpawnCallback = bulletCallBack;
  }

  bool CanShoot() const { return _canShoot; }
};
