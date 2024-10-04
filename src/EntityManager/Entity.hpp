#pragma once
#include "Core/Game/RenderContext.hpp"
#include "raylib.h"
#include <functional>
#include <memory>
class Bullet;
class Player;
class Entity {
protected: // Attributes
  Color _color;
  float _radius;
  Vector2 _velocity;
  float _speed;
  int _acceleration;
  Vector2 _targetVelocity;
  Vector2 _position;

  // Shooting
  bool _canShoot;
  float _fireRate;
  float _fireTimer;
  std::function<void(std::unique_ptr<Bullet> &)> _bulletSpawnCallback;

  // Rotation
  float _rotation;
  float _targetRotation;
  float _rotationAccelertionFactor;
  float _rotationVelocity;
  float _rotationSpeed;

  // State
  bool _isAlve;

  std::function<const Player *()> _getPlayerCallBack;

protected: // Methods
  virtual void UpdateMovement(float dt) = 0;
  virtual void Rotate(float dt) = 0;

public:
  // Core
  Entity(Color color, float speed, int acceleration, Vector2 position)
      : _color(color), _velocity({0}), _speed(speed),
        _acceleration(acceleration), _position(position), _targetVelocity({0}),
        _fireTimer(0), _radius(30), _isAlve(true) {}
  virtual ~Entity() {}
  virtual void Update(float dt, const RenderContext &rendercontext) = 0;
  virtual void Render() = 0;

  // Mutation
  void SetBulletSpawnCallback(
      std::function<void(std::unique_ptr<Bullet> &)> bulletCallBack) {
    _bulletSpawnCallback = bulletCallBack;
  }
  void SetGetPlayerCallBack(std::function<const Player *()> getPlayerCallBack) {
    _getPlayerCallBack = getPlayerCallBack;
  }

  // Access
  Vector2 GetPosition() const { return _position; }

  // State
  virtual bool IsColliding(const Entity &other) = 0;
  bool CanShoot() const { return _canShoot; }
  bool IsAlive() const { return _isAlve; }
};
