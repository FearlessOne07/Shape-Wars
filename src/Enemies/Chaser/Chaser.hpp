#pragma once
#include "EntityManager/Entity.hpp"
#include "EntityManager/EntitySpec.hpp"

class Player;
class Chaser : public Entity {
private:
  void ChasePlayer();
  void UpdateMovement(float dt) override;
  void Rotate(float dt) override;
  void CheckActivity() override;

public:
  Chaser(EntitySpec entitySpec);
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
};
