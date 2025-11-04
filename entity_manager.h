/*
 * Class for holding and managing all entities in the game.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "asset_manager.h"
#include "entity.h"
#include "map.h"
#include "raylib.h"


class EntityManager {
  public:
    EntityManager(
      const std::shared_ptr<AssetManager>& assetManager
    );
    ~EntityManager();

    void cleanup();
    void draw(const std::shared_ptr<AssetManager>& assetManager) const;
    void init();
    bool isPlayerAlive() const;
    void tick(const std::unique_ptr<Map>& map);

  private:
    static constexpr int8_t s_maxEnemyRespawnCooldownFrames { 90 };

    std::shared_ptr<AssetManager>        m_assetManager;
    std::vector<std::unique_ptr<Entity>> m_entities;
    int8_t                               m_enemyRespawnCooldownFrames;
    int8_t                               m_indexOfPlayer;
    bool                                 m_isPlayerAlive { false };
    std::vector<Vector2>                 m_validEnemySpawnPositions;

    void m_addEntity(std::unique_ptr<Entity> newEntity);
    void m_handleEnemyRespawning();
    void m_removeEntityByIndex(int8_t index);
    void m_ySortEntities();
};
