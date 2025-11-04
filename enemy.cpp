#include "enemy.h"

#include "assets.h"
#include "globals.h"


Enemy::Enemy(
  const int16_t x,
  const int16_t y
): Entity(
    Globals::ENEMY_STARTING_HEALTH,
    x,
    y,
    Assets::ZOMBIE_IDLE,
    Assets::ZOMBIE_WALK
) {
  setSpeed(0.5f);
}


/*
 * Get the rectangle used to check for collisions.
 */
Rectangle Enemy::getCollisionShape() const {
  return {
    getX()               + s_leftMargin,
    getY()               + s_topMargin,
    Globals::TILE_WIDTH  - static_cast<float>(s_leftMargin + s_rightMargin),
    Globals::TILE_HEIGHT - static_cast<float>(s_topMargin + s_bottomMargin)
  };
}


EntityType Enemy::getType() const {
  return EntityType::ENEMY;
}


/*
 * Move towards player. Damage player if colliding with them.
 */
void Enemy::tick(
  std::vector<std::unique_ptr<Entity>>& entities,
  const Map& map,
  const int8_t indexOfPlayer,
  const int8_t indexOfSelf
) {
  const std::unique_ptr<Entity>& player { entities[indexOfPlayer] };
  const float diffX { getX() - player->getX() };
  const float diffY { getY() - player->getY() };

  int8_t dirX { 0 };
  int8_t dirY { 0 };
  if(diffX < 0) dirX += 1;
  if(diffX > 0) dirX -= 1;
  if(diffY < 0) dirY += 1;
  if(diffY > 0) dirY -= 1;

  m_moveAndCollide(
    dirX,
    dirY,
    entities,
    map,
    indexOfPlayer,
    indexOfSelf
  );

  if(player->isCollidingWith(getCollisionShape())) player->damage(1);
}
