#include "Bouncer.hpp"
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/EntitySpec.hpp"
#include "Player/Player.hpp"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

Bouncer::Bouncer(EntitySpec spec) : Entity(spec)
{
}

void Bouncer::Bounce(float dt, const RenderContext &rd)
{

  Vector2 minBounds = GetScreenToWorld2D({0, 0}, rd.camera);
  Vector2 maxBounds = GetScreenToWorld2D({rd.gameWidth, rd.gameHeight}, rd.camera);

  if (_position.x > maxBounds.x - _radius)
  {
    _velocity.x = 0;
    _position.x = maxBounds.x - _radius;
    _targetVelocity.x = -_targetVelocity.x;
    _bounceVelocity.x = _targetVelocity.x;
  }
  else if (_position.x < minBounds.x + _radius)
  {
    _velocity.x = 0;
    _position.x = minBounds.x + _radius;
    _targetVelocity.x = -_targetVelocity.x;
    _bounceVelocity.x = _targetVelocity.x;
  }

  if (_position.y > maxBounds.y - _radius)
  {
    _velocity.y = 0;
    _position.y = maxBounds.y - _radius;
    _targetVelocity.y = -_targetVelocity.y;
    _bounceVelocity.y = _targetVelocity.y;
  }
  else if (_position.y < minBounds.y + _radius)
  {
    _velocity.y = 0;
    _position.y = minBounds.y + _radius;
    _targetVelocity.y = -_targetVelocity.y;
    _bounceVelocity.y = _targetVelocity.y;
  }
  _bounceVelocity = Vector2Lerp(_bounceVelocity, {0, 0}, 500 * dt);

  if (Vector2Length(_bounceVelocity) < 0.01)
  {
    _bounceVelocity = {0, 0};
  }
  _velocity = Vector2Add(_bounceVelocity, _velocity);
}

void Bouncer::Update(float dt, const RenderContext &rendercontext)
{

  if (!_hasEntered)
  {
    ApproachPlayer(rendercontext);
  }
  else
  {
    Bounce(dt, rendercontext);
  }
  UpdateMovement(dt);
}
void Bouncer::Render()
{
  DrawCircleV(_position, _radius, BLUE);
}

void Bouncer::ApproachPlayer(const RenderContext &rd)
{

  Vector2 minBounds = GetScreenToWorld2D({0, 0}, rd.camera);
  Vector2 maxBounds = GetScreenToWorld2D({rd.gameWidth, rd.gameHeight}, rd.camera);

  if (                                                                            //
    _position.x < maxBounds.x - _radius && _position.x > minBounds.x + _radius    //
    && _position.y > minBounds.y + _radius && _position.y < maxBounds.y - _radius //
  )
  {
    _hasEntered = true;
  }
  else
  {
    Vector2 playerPos = _getPlayerCallBack()->GetPosition();
    _targetVelocity = Vector2Subtract(playerPos, _position);
  }
}
