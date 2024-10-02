#pragma once
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"

class Player : public Entity {
private:
  void GetInput(const RenderContext &rendercontext);
  void UpdateMovement(float dt) override;
  void Shoot(const RenderContext &rendercontext);
  void UpdateTimers(float dt);

public:
  Player(Color color, float speed, int acceleration, Vector2 position);
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
  bool IsColliding(const Entity &other) override;
};
