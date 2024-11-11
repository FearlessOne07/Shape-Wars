#pragma once
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"
#include "EntityManager/EntitySpec.hpp"

class Player : public Entity {
private:
  void GetInput(const RenderContext &rendercontext);
  void Shoot(const RenderContext &rendercontext);
  void UpdateTimers(float dt);
  void Wrap(const RenderContext &rendercontext);

public:
  Player(EntitySpec entitySpec);
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
};
