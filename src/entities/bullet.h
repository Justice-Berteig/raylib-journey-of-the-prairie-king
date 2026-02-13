/*
 * Class to represent a bullet fired by the player.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "entity.h"
#include "map.h"


class Bullet: public Entity {
  public:
    Bullet(
      const int16_t x,
      const int16_t y,
      const int8_t  xDir,
      const int8_t  yDir
    );

    bool       canCollideWith(const Map& map);
    bool       canCollideWith(const std::unique_ptr<Entity>& other);
    void       collideWith(const Map& map);
    void       collideWith(const std::unique_ptr<Entity>& other);
    Rectangle  getCollisionShape() const;
    EntityType getType() const;
    void       tick(
      const double                          deltaTime,
      std::vector<std::unique_ptr<Entity>>& entities,
      const Map&                            map,
      const int8_t                          indexOfPlayer,
      const int8_t                          indexOfSelf
    );

  private:
    static constexpr int8_t s_topMargin    { 6 };
    static constexpr int8_t s_bottomMargin { 6 };
    static constexpr int8_t s_leftMargin   { 6 };
    static constexpr int8_t s_rightMargin  { 6 };

    int8_t      m_xDir;
    int8_t      m_yDir;
};
