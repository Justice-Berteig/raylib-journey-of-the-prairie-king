/*
 * Basic framework for a game entity.
 * Not meant to be used directly. Meant to be inherited.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "animation.h"
#include "map.h"


class Entity {
  public:
    Entity(
      const int8_t     startingHealth,
      const int16_t    x,
      const int16_t    y,
      const Texture2D& texture,
      const Texture2D& animSheet
    );

    void damage(int8_t amount);
    void draw() const;
    Rectangle getCollisionShape() const;
    float getX() const;
    float getY() const;
    bool isAlive { true };
    bool isCollidingWith(const std::unique_ptr<Entity>& other) const;
    void setSpeed(const float newSpeed);

    virtual void tick(
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map& map,
      const int8_t indexOfPlayer,
      const int8_t indexOfSelf
    ) = 0;

  protected:
    void m_moveAndCollide(
      const int8_t dirX,
      const int8_t dirY,
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map& map,
      const int8_t indexOfPlayer,
      const int8_t indexOfSelf
    );

    int16_t                    m_x;
    int16_t                    m_y;

  private:
    static constexpr int8_t m_topMargin    { 6 };
    static constexpr int8_t m_bottomMargin { 0 };
    static constexpr int8_t m_leftMargin   { 2 };
    static constexpr int8_t m_rightMargin  { 2 };

    int8_t                     m_width;
    int8_t                     m_height;
    int8_t                     m_health;
    int8_t                     m_prevDirX;
    int8_t                     m_prevDirY;
    float                      m_moveSpeed;
    float                      m_diagonalMoveSpeed;
    float                      m_excessMovement;
    bool                       m_isMoving;
    Texture2D                  m_idleSprite;
    std::unique_ptr<Animation> m_walkAnim;

    bool m_isIntersecting(
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map& map,
      const int8_t indexOfPlayer,
      const int8_t indexOfSelf
    ) const;
};
