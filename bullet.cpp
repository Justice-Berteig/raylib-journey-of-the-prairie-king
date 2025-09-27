#include "bullet.h"

#include "globals.h"
#include "utils.h"


Bullet::Bullet(
  const int16_t    x,
  const int16_t    y,
  const int8_t     xDir,
  const int8_t     yDir
): m_xDir(xDir), m_yDir(yDir) {
  m_x     = x + (xDir * 8);
  m_y     = y + (yDir * 8);
  m_speed = (xDir != 0 && yDir != 0) ? m_diagonalMoveSpeed : m_moveSpeed;
}


/*
 * Draw the bullet.
 */
void Bullet::draw(const Texture2D& sprite) {
  drawTextureAt(
    sprite,
    m_x,
    m_y
  );
}


/*
 * Move the bullet.
 * Damage any non-player entities hit.
 * Destroy the bullet if colliding with walls or if outside the map.
 */
void Bullet::moveAndCollide(
  const std::vector<std::unique_ptr<Entity>>& entities,
  const Map& map,
  const int8_t indexOfPlayer
) {
  m_excessMovement += m_speed;

  int8_t amountToMove { 0 };
  while(m_excessMovement >= 1.0f) {
    ++amountToMove;
    m_excessMovement -= 1.0f;
  }

  int16_t velX { static_cast<int16_t>(m_xDir * amountToMove) };
  int16_t velY { static_cast<int16_t>(m_yDir * amountToMove) };

  m_x += velX;
  m_y += velY;

  if(m_isIntersecting(entities, map, indexOfPlayer)) {
    isDestroyed = true;
  }

  // Destroy the bullet if it's out of the map bounds.
  if(
    m_x    < 0 - Globals::TILE_WIDTH
    || m_y < 0 - Globals::TILE_HEIGHT
    || m_x > (Globals::MAP_WIDTH + 1) * Globals::TILE_WIDTH
    || m_y > (Globals::MAP_HEIGHT + 1) * Globals::TILE_HEIGHT
  ) {
    isDestroyed = true;
  }
}


/*
 * Get the rectangle used to check for collisions.
 */
Rectangle Bullet::m_getCollisionShape() const {
  return {
    static_cast<float>(m_x + 6),
    static_cast<float>(m_y + 6),
    4,
    4,
  };
}


/*
 * Check if this bullet is colliding with an entity or the map.
 */
bool Bullet::m_isIntersecting(
  const std::vector<std::unique_ptr<Entity>>& entities,
  const Map& map,
  const int8_t indexOfPlayer
) const {
  const Rectangle selfCollisionShape { m_getCollisionShape() };
  if(map.isCollidingWith(selfCollisionShape)) return true;
  for(int8_t i = 0; i < entities.size(); ++i) {
    if(i == indexOfPlayer) continue;

    const std::unique_ptr<Entity>& e { entities[i] };
    if(e->isCollidingWith(selfCollisionShape)) {
      e->damage(1);
      return true;
    }
  }

  return false;
}
