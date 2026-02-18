#include "entity_manager.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>

#include "globals.h"
#include "player.h"
#include "zombie.h"


EntityManager::EntityManager()
: m_enemyRespawnCooldown(s_MAX_ENEMY_RESPAWN_COOLDOWN)
, m_indexOfPlayer(0)
, m_isPlayerAlive(false)
, m_validEnemySpawnPositions(Map::getValidEnemySpawnPositions())
{
  srand(time(0));
  std::cout << "[EntityManager]: Created.\n";
}


/*
 * Destructor unloads any remaining entities.
 */
EntityManager::~EntityManager() {
  cleanup();

  std::cout << "[EntityManager]: Destroyed.\n";
}


/*
 * Unload all remaining entities.
 */
void EntityManager::cleanup() {
  m_entities.clear();
  m_indexOfPlayer = 0;
}


/*
 * Run the draw() method for every entity in the manager.
 */
void EntityManager::draw(const std::unique_ptr<AssetManager>& assetManager) const {
  for(const std::unique_ptr<Entity>& e : m_entities) {
    e->draw(assetManager);
  }
}


/*
 * Initialize EntityManager with a player.
 */
void EntityManager::init() {
  m_addEntity(std::make_unique<Player>(
    (Globals::MAP_WIDTH / 2) * Globals::TILE_WIDTH,
    (Globals::MAP_HEIGHT / 2) * Globals::TILE_HEIGHT
  ));
  m_isPlayerAlive = true;
}


bool EntityManager::isPlayerAlive() const {
  return m_isPlayerAlive;
}


/*
 * Run the tick() method for every entity in the manager.
 */
void EntityManager::tick(
  const double                deltaTime,
  const std::unique_ptr<Map>& map
) {
  m_handleEnemyRespawning(deltaTime);

  for(int8_t i = 0; i < m_entities.size(); ++i) {
    const std::unique_ptr<Entity>& e { m_entities[i] };
    const int8_t indexOfSelf = i;

    if(!e->getIsDying()) {
      e->tick(deltaTime, m_entities, *map, m_indexOfPlayer, indexOfSelf);
    }else if(indexOfSelf == m_indexOfPlayer) {
      // TODO: Why is this here? Get rid of it!
      m_isPlayerAlive = false;
    }else if(e->isReadyToDie()) {
      m_removeEntityByIndex(indexOfSelf);
    }
  }

  m_ySortEntities();
}


/*
 * Add a new entity to the entity manager.
 */
void EntityManager::m_addEntity(std::unique_ptr<Entity> newEntity) {
  m_entities.push_back(std::move(newEntity));

  std::cout << "[EntityManager]: Added new entity." << '\n';
}


/*
 * Decrement enemy respawn cooldown and spawn a new enemy if the cooldown is finished.
 */
void EntityManager::m_handleEnemyRespawning(const double deltaTime) {
  // If respawning is still on cooldown reduce the cooldown and early return
  if(m_enemyRespawnCooldown > 0) {
    m_enemyRespawnCooldown -= (float)deltaTime;
    return;
  }

  // Spawn an enemy
  // Choose a tile to spawn the enemy on
  const Vector2 spawnPosition {
    m_validEnemySpawnPositions[rand() % m_validEnemySpawnPositions.size()]
  };
  // Add the enemy at the chosen spawn position
  m_addEntity(std::make_unique<Zombie>(
    spawnPosition.x,
    spawnPosition.y
  ));

  // Reset respawn cooldown
  m_enemyRespawnCooldown = (
    s_MIN_ENEMY_RESPAWN_COOLDOWN
    + fmodf(rand(), s_MAX_ENEMY_RESPAWN_RAND_VAL)
  );
}


/*
 * Remove the entity at a given index from this entity manager
 * while keeping the m_indexOfPlayer variable accurate.
 */
void EntityManager::m_removeEntityByIndex(int8_t index) {
  m_entities.erase(m_entities.begin() + index);
  if(index < m_indexOfPlayer) m_indexOfPlayer--;

  std::cout << "[EntityManager]: Removed entity at index " << static_cast<int>(index) << '\n';
}


/*
 * Sort entities in ascending order based on Y position.
 * Also update m_indexOfPlayer as the player is moved around.
 */
void EntityManager::m_ySortEntities() {
  if(m_entities.size() <= 1) return;

  bool isSorted { false };
  while(!isSorted) {
    isSorted = true;
    for(int8_t i = 1; i < m_entities.size(); ++i) {
      const std::unique_ptr<Entity>& a { m_entities[i-1] };
      const std::unique_ptr<Entity>& b { m_entities[i] };

      if(a->getY() > b->getY()) {
        isSorted = false;

        std::unique_ptr<Entity> temp = std::move(m_entities[i-1]);
        m_entities[i-1] = std::move(m_entities[i]);
        m_entities[i]   = std::move(temp);

        if     ((i-1) == m_indexOfPlayer) ++m_indexOfPlayer;
        else if( i    == m_indexOfPlayer) --m_indexOfPlayer;
      } 
    }  
  }
}
