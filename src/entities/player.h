/*
 * Class to represent the player.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "entity.h"
#include "map.h"


class Player: public Entity {
  public:
    Player(
      const int16_t x,
      const int16_t y
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
    // Since deltaTime is ~1 at 60 FPS a cooldown of 60 = 1 second
    static constexpr float  s_MAX_SHOOTING_COOLDOWN { 28.0f };
    static constexpr int8_t s_topMargin    { 6 };
    static constexpr int8_t s_bottomMargin { 0 };
    static constexpr int8_t s_leftMargin   { 2 };
    static constexpr int8_t s_rightMargin  { 2 };

    float m_shootingCooldown;
};
