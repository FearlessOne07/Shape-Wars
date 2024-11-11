#pragma once
#include "EntityManager/Entity.hpp"
#include "EntityManager/EntitySpec.hpp"

class Player;
class Chaser : public Entity {
private:
  void ChasePlayer();

public:
  Chaser(EntitySpec entitySpec);
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
};
