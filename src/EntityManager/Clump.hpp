#pragma once

#include "EntityManager/Entity.hpp"
#include <algorithm>
#include <vector>
class Clump
{
  std::vector<Entity *> _entities;
  Entity *_leader = nullptr;
  Vector2 _centerOfMass = {0, 0};

public:
  std::vector<Entity *>::iterator begin()
  {
    return _entities.begin();
  };
  std::vector<Entity *>::iterator end()
  {
    return _entities.begin();
  };

  void AddEntity(Entity *entity)
  {
    if (!_leader && _entities.empty())
    {
      _leader = entity;
    }
    _entities.push_back(entity);
  }

  void RemoveEntity(Entity *entity)
  {
    auto it = std::find(_entities.begin(), _entities.end(), entity);

    if (it != _entities.end())
    {
      _entities.erase(it);
    }
  }
};
