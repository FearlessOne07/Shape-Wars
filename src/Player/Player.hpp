#pragma once
#include "EntityManager/Entity.hpp"

class Player : public Entity {
private:
  void GetInput();
  void UpdateMovement(float dt);
  void Shoot();

public:
  Player(Color color, float speed, float acceleration, Vector2 position);
  void Update(float dt) override;
  void Render() override;
  bool IsColliding(Entity &other) override;
};
