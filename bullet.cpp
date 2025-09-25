#include "bullet.h"

#include "utils.h"


Bullet::Bullet(
  const int16_t    x,
  const int16_t    y,
  const int8_t     xDir,
  const int8_t     yDir
): m_xDir(xDir), m_yDir(yDir) {
  m_x     = x + (xDir * 8);
  m_y     = y + (yDir * 8);
  m_speed = (xDir != 0 && yDir != 0) ? m_moveSpeed : m_diagonalMoveSpeed;
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

  if(
    m_x < 0
    || m_y < 0
    || m_x > 160
    || m_y > 160
  ) {
    isDestroyed = true;
  }
}
