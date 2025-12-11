/*
 * Basic framework for a game entity.
 * Not meant to be used directly. Meant to be inherited.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "asset_manager.h"
#include "entity_display.h"
#include "map.h"


enum class EntityType {
  PLAYER,
  ZOMBIE,
  BULLET
};


class Entity {
  public:
    void  damage(int8_t amount);
    void  draw(const std::unique_ptr<AssetManager>& assetManager) const;
    bool  getIsDying() const;
    float getX() const;
    float getY() const;
    bool  isCollidingWith(const Rectangle& otherRect) const;
    bool  isReadyToDie() const;
    void  setIsDying(const bool isDying);
    void  setSpeed(const float newSpeed);

    virtual Rectangle  getCollisionShape() const = 0;
    virtual EntityType getType() const = 0;
    virtual void       tick(
      std::vector<std::unique_ptr<Entity>>& entities,
      const Map&                            map,
      const int8_t                          indexOfPlayer,
      const int8_t                          indexOfSelf
    ) = 0;

  protected:
    Entity(
      const int8_t  startingHealth,
      const int16_t x,
      const int16_t y,
      const char    *idleSpritePath,
      const char    *walkSheetPath,
      const char    *deathSheetPath = nullptr
    );

    int16_t m_x;
    int16_t m_y;

    void m_moveAndCollide(
      const int8_t                                dirX,
      const int8_t                                dirY,
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map&                                  map,
      const int8_t                                indexOfPlayer,
      const int8_t                                indexOfSelf
    );

  private:
    int8_t        m_health;
    int8_t        m_prevDirX;
    int8_t        m_prevDirY;
    float         m_moveSpeed;
    float         m_diagonalMoveSpeed;
    float         m_excessMovement;
    bool          m_isDying { false };
    bool          m_isMoving { false };
    EntityDisplay m_display;

    bool m_isIntersecting(
      const std::vector<std::unique_ptr<Entity>>& entities,
      const Map&                                  map,
      const int8_t                                indexOfPlayer,
      const int8_t                                indexOfSelf
    ) const;
};
