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

void Bouncer::Bounce(const RenderContext &rd)
{

  Vector2 minBounds = GetScreenToWorld2D({0, 0}, rd.camera);
  Vector2 maxBounds = GetScreenToWorld2D({rd.gameWidth, rd.gameHeight}, rd.camera);

  if (_position.x >= maxBounds.x - _radius)
  {
    _bounceVelocity.x = -1;
    _targetVelocity.x *= -1;
  }
  else if (_position.x <= minBounds.x + _radius)
  {
    _bounceVelocity.x = 1;
    _targetVelocity.x *= -1;
  }

  if (_position.y >= maxBounds.y - _radius)
  {
    _bounceVelocity.y = -1;
    _targetVelocity.y *= -1;
  }
  else if (_position.y <= minBounds.y + _radius)
  {
    _bounceVelocity.y = 1;
    _targetVelocity.y *= -1;
  }

  _bounceVelocity = Vector2Scale(_bounceVelocity, _bounceForce);
}

void Bouncer::Update(float dt, const RenderContext &rendercontext)
{

  if (!_hasEntered)
  {
    ApproachPlayer(rendercontext);
  }
  else
  {
    Bounce(rendercontext);
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
