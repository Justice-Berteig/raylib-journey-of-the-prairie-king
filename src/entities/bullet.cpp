#include "bullet.h"

#include "assets/assets.h"
#include "globals.h"


Bullet::Bullet(
  const int16_t x,
  const int16_t y,
  const int8_t  xDir,
  const int8_t  yDir
)
: Entity(
  Globals::BULLET_STARTING_HEALTH,
  x,
  y,
  Assets::BULLET,
  nullptr
)
, m_xDir(xDir)
, m_yDir(yDir)
{
  m_x = x + (xDir * Globals::TILE_WIDTH  / 2);
  m_y = y + (yDir * Globals::TILE_HEIGHT / 2);
  setSpeed(2.0f);
}


/*
 * Get the rectangle used to check for collisions.
 */
Rectangle Bullet::getCollisionShape() const {
  return {
    getX()               + s_leftMargin,
    getY()               + s_topMargin,
    Globals::TILE_WIDTH  - static_cast<float>(s_leftMargin + s_rightMargin),
    Globals::TILE_HEIGHT - static_cast<float>(s_topMargin + s_bottomMargin)
  };
}


EntityType Bullet::getType() const {
  return EntityType::BULLET;
}


void Bullet::tick(
  std::vector<std::unique_ptr<Entity>>& entities,
  const Map&                            map,
  const int8_t                          indexOfPlayer,
  const int8_t                          indexOfSelf
) {
  m_moveAndCollide(
    m_xDir,
    m_yDir,
    entities,
    map,
    indexOfPlayer,
    indexOfSelf
  );
}
