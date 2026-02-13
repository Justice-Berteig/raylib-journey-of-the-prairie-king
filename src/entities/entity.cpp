#include "entity.h"


Entity::Entity(
  const int8_t  startingHealth,
  const int16_t x,
  const int16_t y,
  const char    *idleSpritePath,
  const char    *walkSheetPath,
  const char    *deathSheetPath
)
: m_health(startingHealth)
, m_x(x)
, m_y(y)
, m_display(
    idleSpritePath,
    walkSheetPath,
    deathSheetPath
  )
{}


/*
 * Damage the entity by a given amount. If health drops to 0, mark the entity as
 * no longer alive.
 */
void Entity::damage(int8_t amount) {
  m_health -= amount;
  if(m_health <= 0) {
    m_isDying = true;
    m_display.startDeathAnim();
  }
}


/*
 * Draw the entity using either the idle sprite or walking animation.
 */
void Entity::draw(const std::unique_ptr<AssetManager>& assetManager) const {
  m_display.draw(
    assetManager,
    m_isDying,
    m_isMoving,
    getX(),
    getY()
  );
}


bool Entity::getIsDying() const {
  return m_isDying;
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
bool Entity::isCollidingWith(const Rectangle& otherRect) const {
  const Rectangle a { getCollisionShape() };
  const Rectangle b { otherRect };

  bool isOverlappingOnX { true };
  if(
       a.x + a.width - 1 < b.x
    || a.x > b.x + b.width - 1
  ) isOverlappingOnX = false;

  bool isOverlappingOnY { true };
  if(
       a.y + a.height - 1 < b.y
    || a.y > b.y + b.height - 1
  ) isOverlappingOnY = false;

  return isOverlappingOnX && isOverlappingOnY;
}


/*
 * Check if the entity is ready to be removed from the game.
 */
bool Entity::isReadyToDie() const {
  if(m_isDying && m_display.isReadyToDie(m_isDying)) return true;
  return false;
}


void Entity::setIsDying(const bool isDying) {
  m_isDying = isDying;
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
  const double                                deltaTime,
  const int8_t                                xDir,
  const int8_t                                yDir,
  const std::vector<std::unique_ptr<Entity>>& entities,
  const Map&                                  map,
  const int8_t                                indexOfPlayer,
  const int8_t                                indexOfSelf
) {
  // Compare with previous direction and reset excess movement if different
  if(xDir != m_prevDirX || yDir != m_prevDirY) {
    m_excessMovement = 0.0f;
  }

  m_prevDirX = xDir;
  m_prevDirY = yDir;

  // Check if the entity is actually moving
  // TODO: Might still be colliding even if not moving, and should be moved away
  if(xDir == 0 && yDir == 0) {
    m_isMoving = false;
    return;
  }

  m_isMoving = true;

  // Check if entity is currently colliding with something
  if(m_isIntersecting(entities, map, indexOfPlayer, indexOfSelf)) return;

  int16_t initialX { m_x };
  int16_t initialY { m_y };
  float   speed    {
    (
     (xDir != 0 && yDir != 0)
      ? m_diagonalMoveSpeed
      : m_moveSpeed
    ) * (float)deltaTime
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

  // Apply movement
  m_x += velX;
  m_y += velY;

  // Check collisions again
  if(!m_isIntersecting(entities, map, indexOfPlayer, indexOfSelf)) return;
  else {
    // If the entity collided with something and is not a bullet
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

  // Destroy the entity if it is off screen
  if(
       m_x < 0 - Globals::TILE_WIDTH
    || m_y < 0 - Globals::TILE_HEIGHT
    || m_x > (Globals::MAP_WIDTH  - 1) * Globals::TILE_WIDTH
    || m_y > (Globals::MAP_HEIGHT - 1) * Globals::TILE_HEIGHT
  ) {
    m_isDying = true;
  }
}


/*
 * Check if this entity is colliding with any other entity or the map.
 */
bool Entity::m_isIntersecting(
  const std::vector<std::unique_ptr<Entity>>& entities,
  const Map&                                  map,
  const int8_t                                indexOfPlayer,
  const int8_t                                indexOfSelf
) const {
  const std::unique_ptr<Entity>& self { entities[indexOfSelf] };

  // Check for collisions with the map
  if(
    self->canCollideWith(map)
    && map.isCollidingWith(self->getCollisionShape())
  ) {
    self->collideWith(map);
    return true;
  }

  // Check for collisions with entities
  for(int8_t i = 0; i < entities.size(); ++i) {
    // Skip checking collision with self.
    if(i == indexOfSelf) continue;

    const std::unique_ptr<Entity>& other { entities[i] };
    if(
      !other->getIsDying()
      && self->canCollideWith(other)
      && self->isCollidingWith(other->getCollisionShape())
    ) {
      self->collideWith(other);
      return true;
    }
  }

  return false;
}
