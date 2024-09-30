#pragma once
#include "Core/Game/RenderContext.hpp"
#include "EntityManager/Entity.hpp"

class Player : public Entity {
private:
  void GetInput(const RenderContext &rendercontext);
  void UpdateMovement(float dt);
  void Shoot(const RenderContext &rendercontext);

public:
  Player(Color color, float speed, float acceleration, Vector2 position);
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
  bool IsColliding(Entity &other) override;
};
