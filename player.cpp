#include "player.h"

#include "globals.h"


Player::Player(
  const int16_t x,
  const int16_t y,
  const Texture2D& texture,
  const Texture2D& animSheet,
  std::vector<std::unique_ptr<Bullet>>& bullets
): Entity(Globals::PLAYER_STARTING_HEALTH, x, y, texture, animSheet)
{
  m_bullets = std::move(bullets);
  setSpeed(1.0f);
}


/*
 * Move based on player input.
 */
void Player::tick(
  const std::vector<std::unique_ptr<Entity>>& entities,
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

  int8_t xShootingDir { 0 };
  int8_t yShootingDir { 0 };
  if(IsKeyDown(KEY_LEFT))  xShootingDir -= 1;
  if(IsKeyDown(KEY_RIGHT)) xShootingDir += 1;
  if(IsKeyDown(KEY_UP))    xShootingDir -= 1;
  if(IsKeyDown(KEY_DOWN))  xShootingDir += 1;
  if(xShootingDir != 0 || yShootingDir != 0) {
    m_bullets.push_back(std::make_unique<Bullet>(
      m_x,
      m_y,
      xShootingDir,
      yShootingDir
    ));
  }
}
