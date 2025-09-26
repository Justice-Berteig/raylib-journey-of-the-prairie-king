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

class Bullet {
  public:
    Bullet(
      const int16_t x,
      const int16_t y,
      const int8_t  xDir,
      const int8_t  yDir
    );

    void draw(const Texture2D& sprite);
    void moveAndCollide(
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map& map,
      const int8_t indexOfPlayer
    );

    bool isDestroyed { false };

  private:
    static constexpr float m_moveSpeed         { 2.0f };
    static constexpr float m_diagonalMoveSpeed { (m_moveSpeed * 2) / 3 };

    int16_t     m_x;
    int16_t     m_y;
    int8_t      m_xDir;
    int8_t      m_yDir;
    float       m_excessMovement;
    float       m_speed;

    Rectangle m_getCollisionShape() const;
    bool      m_isIntersecting(
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map& map,
      const int8_t indexOfPlayer
    ) const;
};
