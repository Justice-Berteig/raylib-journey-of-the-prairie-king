#include "player.h"

#include "assets/assets.h"
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
, m_shootingCooldown(0)
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
  const double                          deltaTime,
  std::vector<std::unique_ptr<Entity>>& entities,
  const Map&                            map,
  const int8_t                          indexOfPlayer,
  const int8_t                          indexOfSelf
) {
  // Get movement direction from keyboard input
  // and move the player in that direction
  int8_t dirX { 0 };
  int8_t dirY { 0 };
  if(IsKeyDown(KEY_A)) dirX -= 1;
  if(IsKeyDown(KEY_D)) dirX += 1;
  if(IsKeyDown(KEY_W)) dirY -= 1;
  if(IsKeyDown(KEY_S)) dirY += 1;

  m_moveAndCollide(
    deltaTime,
    dirX,
    dirY,
    entities,
    map,
    indexOfPlayer,
    indexOfSelf
  );

  // Reduce shooting cooldown
  m_shootingCooldown -= (float)deltaTime;

  if(m_shootingCooldown <= 0) {
    // If the player can shoot
    // get shooting direction from keyboard input
    int8_t xShootingDir { 0 };
    int8_t yShootingDir { 0 };
    if(IsKeyDown(KEY_LEFT))  xShootingDir -= 1;
    if(IsKeyDown(KEY_RIGHT)) xShootingDir += 1;
    if(IsKeyDown(KEY_UP))    yShootingDir -= 1;
    if(IsKeyDown(KEY_DOWN))  yShootingDir += 1;

    if(xShootingDir != 0 || yShootingDir != 0) {
      // If the player is trying to shoot
      // create a bullet and reset the shooting cooldown
      entities.push_back(std::make_unique<Bullet>(
        m_x,
        m_y,
        xShootingDir,
        yShootingDir
      ));
      m_shootingCooldown = s_MAX_SHOOTING_COOLDOWN;
    }
  }
}
