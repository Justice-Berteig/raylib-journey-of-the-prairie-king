#include "player.h"

#include "assets.h"
#include "bullet.h"
#include "globals.h"


Player::Player(
  const int16_t x,
  const int16_t y
)
: Entity(
  Globals::PLAYER_STARTING_HEALTH,
  x,
  y,
  Assets::PLAYER_IDLE,
  Assets::PLAYER_WALK
)
, m_shootingCooldownFrames(0)
{
  setSpeed(1.0f);
}


/*
 * Get the rectangle used to check for collisions.
 */
Rectangle Player::getCollisionShape() const {
  return {
    getX()               + s_leftMargin,
    getY()               + s_topMargin,
    Globals::TILE_WIDTH  - static_cast<float>(s_leftMargin + s_rightMargin),
    Globals::TILE_HEIGHT - static_cast<float>(s_topMargin + s_bottomMargin)
  };
}


EntityType Player::getType() const {
  return EntityType::PLAYER;
}


/*
 * Move based on player input.
 */
void Player::tick(
  std::vector<std::unique_ptr<Entity>>& entities,
  const Map& map,
  const int8_t indexOfPlayer,
  const int8_t indexOfSelf
) {
  int8_t dirX { 0 };
  int8_t dirY { 0 };
  if(IsKeyDown(KEY_A)) dirX -= 1;
  if(IsKeyDown(KEY_D)) dirX += 1;
  if(IsKeyDown(KEY_W)) dirY -= 1;
  if(IsKeyDown(KEY_S)) dirY += 1;

  m_moveAndCollide(
    dirX,
    dirY,
    entities,
    map,
    indexOfPlayer,
    indexOfSelf
  );

  if(m_shootingCooldownFrames > 0) {
    --m_shootingCooldownFrames;
  }

  int8_t xShootingDir { 0 };
  int8_t yShootingDir { 0 };
  if(IsKeyDown(KEY_LEFT))  xShootingDir -= 1;
  if(IsKeyDown(KEY_RIGHT)) xShootingDir += 1;
  if(IsKeyDown(KEY_UP))    yShootingDir -= 1;
  if(IsKeyDown(KEY_DOWN))  yShootingDir += 1;
  if(
    m_shootingCooldownFrames == 0
    && (xShootingDir != 0 || yShootingDir != 0)
  ) {
    entities.push_back(std::make_unique<Bullet>(
      m_x,
      m_y,
      xShootingDir,
      yShootingDir
    ));
    m_shootingCooldownFrames = s_maxShootingCooldownFrames;
  }
}
