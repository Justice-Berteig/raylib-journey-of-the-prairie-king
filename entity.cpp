#include "entity.h"

#include <iostream>

#include "utils.h"


Entity::Entity(
  const int8_t     startingHealth,
  const int16_t    x,
  const int16_t    y,
  const Texture2D& texture,
  const Texture2D& animSheet
)
: m_health(startingHealth),
  m_x(x),
  m_y(y),
  m_isMoving(false),
  m_idleSprite(texture)
{
  m_width = m_idleSprite.width;
  m_height = m_idleSprite.height;

  m_walkAnim.reset(new Animation(animSheet));
}


/*
 * Damage the entity by a given amount. If health drops to 0, mark the entity as
 * no longer alive.
 */
void Entity::damage(int8_t amount) {
  m_health -= amount;
  if(m_health <= 0) isAlive = false;
}


/*
 * Draw the entity using either the idle sprite or walking animation.
 */
void Entity::draw() const {
  if(m_isMoving) {
    m_walkAnim->drawCurrentFrameAt(
      getX(),
      getY()
    );
  }else {
    drawTextureAt(
      m_idleSprite,
      getX(),
      getY()
    );
  }
}


/*
 * Get the rectangle used to check for collisions.
 */
Rectangle Entity::getCollisionShape() const {
  return {
    getX()   + m_leftMargin,
    getY()   + m_topMargin,
    m_width  - (float)(m_leftMargin + m_rightMargin),
    m_height - (float)(m_topMargin + m_bottomMargin)
  };
}


float Entity::getX() const {
  return static_cast<float>(m_x);
}


float Entity::getY() const {
  return static_cast<float>(m_y);
}


/*
 * Check if this entity is colliding with another entity.
 */
bool Entity::isCollidingWith(const std::unique_ptr<Entity>& other) const {
  Rectangle a { getCollisionShape() };
  Rectangle b { other->getCollisionShape() };

  if(a.x == b.x && a.y == b.y) return false;

  bool isOverlappingOnX { true };
  if(a.x + a.width < b.x || a.x > b.x + b.width) isOverlappingOnX = false;

  bool isOverlappingOnY { true };
  if(a.y + a.height < b.y || a.y > b.y + b.height) isOverlappingOnY = false;

  return isOverlappingOnX && isOverlappingOnY;
}


void Entity::setSpeed(const float newSpeed) {
  m_moveSpeed         = newSpeed;
  m_diagonalMoveSpeed = (newSpeed * 2) / 3;
}


/*
 * Move the entity by a given amount.
 * Check for collisions with the map and other entities.
 */
void Entity::m_moveAndCollide(
  const int8_t xDir,
  const int8_t yDir,
  const std::vector<std::unique_ptr<Entity>>& entities,
  const Map& map,
  const int8_t indexOfPlayer,
  const int8_t indexOfSelf
) {
  // Compare with previous direction and reset excess movement if different
  if(xDir != m_prevDirX || yDir != m_prevDirY) {
    m_excessMovement = 0.0f;
  }

  m_prevDirX = xDir;
  m_prevDirY = yDir;

  // Check if the entity is actually moving
  if(xDir == 0 && yDir == 0) {
    m_isMoving = false;
    return;
  }

  // If the entity is actually moving
  m_isMoving = true;

  int16_t initialX { m_x };
  int16_t initialY { m_y };
  float   speed    {
    (xDir != 0 && yDir != 0)
    ? m_diagonalMoveSpeed
    : m_moveSpeed
  };
 
  // Get the amount the entity is supposed to move
  m_excessMovement += speed;

  int8_t amountToMove { 0 };
  while(m_excessMovement >= 1.0f) {
    ++amountToMove;
    m_excessMovement -= 1.0f;
  }

  int16_t velX { static_cast<int16_t>(xDir * amountToMove) };
  int16_t velY { static_cast<int16_t>(yDir * amountToMove) };

  m_x += velX;
  m_y += velY;

  if(!m_isIntersecting(entities, map, indexOfPlayer, indexOfSelf)) return;

  // If the entity collided with something
  m_x = initialX;
  m_y = initialY;
  int16_t increment { 0 };

  // Apply movement in only x direction, then move back in increments of 1 until
  // the entity is no longer colliding with anything
  m_x += velX;

  if(velX > 0) increment = 1;
  else increment = -1;
  while(m_isIntersecting(entities, map, indexOfPlayer, indexOfSelf)) {
    m_x -= increment;
  }

  // Do the same but in the y direction now
  m_y += velY;

  if(velY > 0) increment = 1;
  else increment = -1;
  while(m_isIntersecting(entities, map, indexOfPlayer, indexOfSelf)) {
    m_y -= increment;
  }
}


/*
 * Check if this entity is colliding with any other entity or the map.
 */
bool Entity::m_isIntersecting(
  const std::vector<std::unique_ptr<Entity>>& entities,
  const Map& map,
  const int8_t indexOfPlayer,
  const int8_t indexOfSelf
) const {
  if(map.isCollidingWith(getCollisionShape())) return true;
  for(int8_t i = 0; i < entities.size(); ++i) {
    if(i == indexOfSelf || i == indexOfPlayer) continue;

    const std::unique_ptr<Entity>& e { entities[i] };
    if(isCollidingWith(e)) return true;
  }

  return false;
}
