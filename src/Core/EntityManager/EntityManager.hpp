#pragma once

#include "Core/EntityManager/Entity.hpp"
#include <memory>
#include <vector>
class EntityManager {
private:
  std::vector<std::unique_ptr<Entity>> _entities;
};
