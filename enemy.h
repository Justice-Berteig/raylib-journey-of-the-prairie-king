/*
      const Texture2D& texture,
      const Texture2D& animSheet
 * Class to represent an enemy.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "entity.h"
#include "map.h"


class Enemy: public Entity {
  public:
    Enemy(
      const int16_t x,
      const int16_t y
    );

    EntityType getType() const;
    void       tick(
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map& map,
      const int8_t indexOfPlayer,
      const int8_t indexOfSelf
    );
};
