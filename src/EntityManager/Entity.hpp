#pragma once
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "raylib.h"
#include <functional>
#include <memory>
class Bullet;
class Player;
class Entity {
protected:
  // Initialized Attributes
  Color _color;
  float _radius;
  float _speed;
  int _acceleration;
  float _rotationSpeed;
  int _healthPoints;
  bool _canShoot;

  float _fireRate;
  Vector2 _velocity;
  Vector2 _targetVelocity;
  Vector2 _position;

  // Shooting
  float _fireTimer;
  std::function<void(std::unique_ptr<Bullet> &)> _bulletSpawnCallback;

  // Rotation
  float _rotation;
  float _targetRotation;
  float _rotationAccelertionFactor;
  float _rotationVelocity;

  // State
  bool _isAlve;

  std::function<const Player *()> _getPlayerCallBack;

protected: // Methods
  virtual void UpdateMovement(float dt) = 0;
  virtual void Rotate(float dt) = 0;
  virtual void CheckActivity() = 0;

public:
  // Core
  Entity(EntitySpec entitySpec) {

    _isAlve = true;
    // Read-in
    _radius = entitySpec.radius;
    _speed = entitySpec.speed;
    _acceleration = entitySpec.acceleration;
    _rotationSpeed = entitySpec.rotationSpeed;
    _healthPoints = entitySpec.healthPoints;
    _canShoot = entitySpec.canShoot;
    _fireRate = entitySpec.fireRate;

    // Determined
    _position = entitySpec.position;
  }
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
  float GetRadius() const { return _radius; }

  // State
  bool CanShoot() const { return _canShoot; }
  bool IsAlive() const { return _isAlve; }

  int GetHp() const { return _healthPoints; }
  void SetHp(int newHp) { _healthPoints = newHp; }
};
