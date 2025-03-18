#include "base/System.hpp"

class BulletSystem : public Base::System
{
  void Stop() override;
  void Start() override;

public:
  void Update(float dt, Base::EntityManager *entityManager) override;
};
