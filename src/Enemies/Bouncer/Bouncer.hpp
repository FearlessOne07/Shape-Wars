#pragma once
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "EntityManager/EntitySpec.hpp"
class Bouncer : public Entity
{
private:
  Vector2 _bounceVelocity = {0.f, 0.f};
  float _bounceForce = 0.f;

  bool _hasEntered = false;

private:
  void Bounce(const RenderContext &rd);
  void ApproachPlayer(const RenderContext &rd);

public:
  Bouncer(EntitySpec spec);
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
};
