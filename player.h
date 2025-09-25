/*
 * Class to represent the player.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "bullet.h"
#include "entity.h"
#include "map.h"


class Player: public Entity {
  public:    
    Player(
      const int16_t x,
      const int16_t y,
      const Texture2D& texture,
      const Texture2D& animSheet,
      const std::vector<std::unique_ptr<Bullet>>& bullets
    );

    void tick(
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map& map,
      const int8_t indexOfPlayer,
      const int8_t indexOfSelf
    );

  private:
    const std::vector<std::unique_ptr<Bullet>> m_bullets;
};
