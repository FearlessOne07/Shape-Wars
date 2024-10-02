#pragma once
#include "EntityManager/Entity.hpp"
#include <functional>

class Player;
class Chaser : public Entity {
private:
  void ChasePlayer();
  void UpdateMovement(float dt) override;

public:
  Chaser(Color color, float speed, int acceleration, Vector2 position);
  void Update(float dt, const RenderContext &rendercontext) override;
  void Render() override;
  bool IsColliding(const Entity &other) override;

};
