#pragma once
#include "EntityManager/Entity.hpp"

class Shooter : public Entity {

private:
  bool _hasPlanted = false;

private:
  void UpdateMovement(float dt) override;
  void Rotate(float dt) override;
  void CheckActivity() override;

  void ApproachPlayer(float dt);
  void Shoot(float dt);

public:
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
  Shooter(EntitySpec entitySpec);
};
