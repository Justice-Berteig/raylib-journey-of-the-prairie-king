#include "entity_manager.h"

#include <iostream>


EntityManager::EntityManager(): indexOfPlayer(0) {
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
  indexOfPlayer = 0;
}


/*
 * Run the draw() method for every entity in the manager.
 */
void EntityManager::draw() {
  for(std::unique_ptr<Entity>& e : m_entities) {
    e->draw();
  }
}


/*
 * Return a const reference to the vector of entities so that other
 * classes can get information about the entities without modifying them.
 */
const std::vector<std::unique_ptr<Entity>>& EntityManager::getEntities() const {
  const std::vector<std::unique_ptr<Entity>>& constEntities { m_entities };
  return constEntities;
}


/*
 * Reset the Entity manager by removing all existing entities
 * then adding back a new player.
 */
void EntityManager::reset() {
  cleanup();
}


/*
 * Run the tick() method for every entity in the manager.
 */
void EntityManager::tick(const std::unique_ptr<Map>& map) {
  for(int8_t i = 0; i < m_entities.size(); ++i) {
    const std::unique_ptr<Entity>& e { m_entities[i] };
    const int8_t indexOfSelf = i;

    if(e->isAlive) {
      e->tick(m_entities, *map, indexOfPlayer, indexOfSelf);
    }else if(indexOfSelf == indexOfPlayer) {
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
void EntityManager::addEntity(std::unique_ptr<Entity> newEntity) {
  m_entities.push_back(std::move(newEntity));

  std::cout << "[EntityManager]: Added new entity." << '\n';
}


/*
 * Remove the entity at a given index from this entity manager
 * while keeping the indexOfPlayer variable accurate.
 */
void EntityManager::m_removeEntityByIndex(int8_t index) {
  m_entities.erase(m_entities.begin() + index);
  if(index < indexOfPlayer) indexOfPlayer--;

  std::cout << "[EntityManager]: Removed entity at index " << static_cast<int>(index) << '\n';
}


/*
 * Sort entities in ascending order based on Y position.
 * Also update indexOfPlayer as the player is moved around.
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

        if     ((i-1) == indexOfPlayer) ++indexOfPlayer;
        else if( i    == indexOfPlayer) --indexOfPlayer;
      } 
    }  
  }
}
