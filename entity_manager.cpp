#include "entity_manager.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "assets.h"
#include "enemy.h"
#include "globals.h"
#include "player.h"


EntityManager::EntityManager(
  const std::shared_ptr<AssetManager>& assetManager
): m_indexOfPlayer(0),
   m_assetManager(std::move(assetManager)),
   m_validEnemySpawnPositions(Globals::getValidEnemySpawnPositions())
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
void EntityManager::draw(const std::shared_ptr<AssetManager>& assetManager) const {
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
  isPlayerAlive = true;
}


/*
 * Run the tick() method for every entity in the manager.
 */
void EntityManager::tick(const std::unique_ptr<Map>& map) {
  m_handleEnemyRespawning();

  for(int8_t i = 0; i < m_entities.size(); ++i) {
    const std::unique_ptr<Entity>& e { m_entities[i] };
    const int8_t indexOfSelf = i;

    if(e->isAlive) {
      e->tick(m_entities, *map, m_indexOfPlayer, indexOfSelf);
    }else if(indexOfSelf == m_indexOfPlayer) {
      isPlayerAlive = false;
    }else {
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
void EntityManager::m_handleEnemyRespawning() {
  if(m_enemyRespawnCooldownFrames > 0) {
    --m_enemyRespawnCooldownFrames;
  }else {
    // Spawn an enemy
    // Choose a tile to spawn the enemy on
    const Vector2 spawnPosition { m_validEnemySpawnPositions[rand() % m_validEnemySpawnPositions.size()] };
    m_addEntity(std::make_unique<Enemy>(
      spawnPosition.x,
      spawnPosition.y
    ));
    m_enemyRespawnCooldownFrames = 24 + (rand() % 64);
  }
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
