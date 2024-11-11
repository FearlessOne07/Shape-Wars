#pragma once
#include "EntityManager/Entity.hpp"

class Shooter : public Entity {

private:
  bool _hasPlanted = false;
  float _plantDistance = 500.f;

private:
  void ApproachPlayer(float dt);
  void Attack(float dt);

public:
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
  Shooter(EntitySpec entitySpec);
};
