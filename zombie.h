/*
 * Class to represent an enemy.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "entity.h"
#include "map.h"


class Zombie: public Entity {
  public:
    Zombie(
      const int16_t x,
      const int16_t y
    );

    Rectangle  getCollisionShape() const;
    EntityType getType() const;
    void       tick(
      std::vector<std::unique_ptr<Entity>>& entities,
      const Map& map,
      const int8_t indexOfPlayer,
      const int8_t indexOfSelf
    );

  private:
    static constexpr int8_t s_topMargin    { 6 };
    static constexpr int8_t s_bottomMargin { 0 };
    static constexpr int8_t s_leftMargin   { 2 };
    static constexpr int8_t s_rightMargin  { 2 };
};
