#pragma once
#include "Core/EntityManager/Entity.hpp"

class Player : public Entity {
private:
  void GetInput();
  void UpdateMovement(float dt);

public:
  Player(Color color, float speed, float acceleration, Vector2 position);
  void Update(float dt) override;
  void Render() override;
};
