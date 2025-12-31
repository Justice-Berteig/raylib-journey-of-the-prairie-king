/*
 * Class for holding and managing all entities in the game.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "assets/asset_manager.h"
#include "entity.h"
#include "map.h"


class EntityManager {
  public:
    EntityManager();
    ~EntityManager();

    void cleanup();
    void draw(const std::unique_ptr<AssetManager>& assetManager) const;
    void init();
    bool isPlayerAlive() const;
    void tick(const double deltaTime, const std::unique_ptr<Map>& map);

  private:
    // Since deltaTime is ~1 at 60 FPS a cooldown of 60 = 1 second
    static constexpr float s_MAX_ENEMY_RESPAWN_COOLDOWN { 90.0f };
    static constexpr float s_MIN_ENEMY_RESPAWN_COOLDOWN { 24.0f };
    static constexpr float s_MAX_ENEMY_RESPAWN_RAND_VAL {
      s_MAX_ENEMY_RESPAWN_COOLDOWN - s_MIN_ENEMY_RESPAWN_COOLDOWN
    };

    std::vector<std::unique_ptr<Entity>> m_entities;
    float                                m_enemyRespawnCooldown;
    int8_t                               m_indexOfPlayer;
    bool                                 m_isPlayerAlive;
    std::vector<Vector2>                 m_validEnemySpawnPositions;

    void m_addEntity(std::unique_ptr<Entity> newEntity);
    void m_handleEnemyRespawning(const double deltaTime);
    void m_removeEntityByIndex(int8_t index);
    void m_ySortEntities();
};
