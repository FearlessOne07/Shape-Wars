#pragma once
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "Utils/Damage.hpp"
#include "raylib.h"
#include "raymath.h"
#include <functional>
#include <iostream>
#include <memory>
class Bullet;
class Player;
class Entity
{
protected:
  // Initialized Attributes
  Color _color = WHITE;
  float _radius = 0.f;
  float _speed = 0.f;
  int _acceleration = 0.f;
  float _rotationSpeed = 0.f;
  int _healthPoints = 0;
  bool _canShoot = false;
  Damage _damage;

  float _fireRate = 0.f;
  Vector2 _velocity = {0.f};
  Vector2 _targetVelocity = {0.f};
  Vector2 _position = {0.f};

  Vector2 _knockbackVelocity = {0.f};
  float _knockbackDecay = 1500.f; // Controls how quickly the knockback fades

  // Shooting
  float _fireTimer = {0.f};
  std::function<void(std::unique_ptr<Bullet> &)> _bulletSpawnCallback;

  // Rotation
  float _rotation = 0.f;
  float _targetRotation = 0.f;
  float _rotationAccelertionFactor = 0.f;
  float _rotationVelocity = 0.f;

  // State
  bool _isAlve = true;

  std::function<const Player *()> _getPlayerCallBack;

protected: // Methods
  void UpdateMovement(float dt)
  {
    if (Vector2Length(_targetVelocity) > 0)
    {
      _targetVelocity = Vector2Normalize(_targetVelocity);
    }

    _velocity = Vector2Lerp(_velocity, _targetVelocity, _acceleration * dt);

    // Apply knockback velocity and decay it
    _velocity = Vector2Add(_velocity, _knockbackVelocity);
    _knockbackVelocity = Vector2Lerp(_knockbackVelocity, {0.f, 0.f}, _knockbackDecay * dt);

    if (Vector2Length(_knockbackVelocity) <= 0.01)
    {
      _knockbackVelocity = {0, 0};
    }

    _position = Vector2Add(_position, Vector2Scale(_velocity, _speed * dt));
  }

  void CheckActivity()
  {
    if (_healthPoints <= 0)
    {
      _isAlve = false;
    }
  }

  void Rotate(float dt)
  {
    if (_targetRotation >= 0)
    {
      _rotationVelocity = Lerp(_rotationVelocity, _targetRotation, _rotationAccelertionFactor * dt);
    }
    _rotation += (_rotationVelocity * _rotationSpeed) * dt;

    if (_rotation > 360)
    {
      _rotation = 0;
    }
  }

  void TakeKnockBack(Vector2 direction, float force)
  {
    if (Vector2Length(direction) > 0)
    {
      direction = Vector2Normalize(direction);
    }
    _knockbackVelocity = direction;
  }

public:
  // Core
  Entity(EntitySpec entitySpec)
  {

    _isAlve = true;
    // Read-in
    _radius = entitySpec.radius;
    _speed = entitySpec.speed;
    _acceleration = entitySpec.acceleration;
    _rotationSpeed = entitySpec.rotationSpeed;
    _healthPoints = entitySpec.healthPoints;
    _canShoot = entitySpec.canShoot;
    _fireRate = entitySpec.fireRate;
    _damage = entitySpec.damage;

    // Determined
    _position = entitySpec.position;
  }
  virtual ~Entity()
  {
  }
  virtual void Update(float dt, const RenderContext &rendercontext) = 0;
  virtual void Render() = 0;

  // Mutation
  void SetBulletSpawnCallback(std::function<void(std::unique_ptr<Bullet> &)> bulletCallBack)
  {
    _bulletSpawnCallback = bulletCallBack;
  }

  void SetGetPlayerCallBack(std::function<const Player *()> getPlayerCallBack)
  {
    _getPlayerCallBack = getPlayerCallBack;
  }

  // Access
  Vector2 GetPosition() const
  {
    return _position;
  }
  float GetRadius() const
  {
    return _radius;
  }

  // State
  bool CanShoot() const
  {
    return _canShoot;
  }
  bool IsAlive() const
  {
    return _isAlve;
  }

  int GetHp() const
  {
    return _healthPoints;
  }
  void TakeDamage(const Damage &damage)
  {
    if (damage.hitPoints > _healthPoints)
    {
      _healthPoints = 0;
    }
    else
    {
      _healthPoints -= damage.hitPoints;
    }

    if (Vector2Length(damage.direction) > 0 && damage.force > 0.f)
    {
      TakeKnockBack(damage.direction, damage.force);
    }
  }

  const Damage &GetDamage()
  {
    _damage.direction = _velocity;
    return _damage;
  }
};
