#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "asset_manager.h"
#include "bullet.h"
#include "entity.h"
#include "globals.h"
#include "map.h"


class Game {
  public:
    Game();
    ~Game();

    void run();

  private:
    // Define width and height of render texture in pixels
    static constexpr int16_t s_RENDER_TEXTURE_WIDTH {
      Globals::MAP_WIDTH * Globals::TILE_WIDTH
    };
    static constexpr int16_t s_RENDER_TEXTURE_HEIGHT {
      Globals::MAP_HEIGHT * Globals::TILE_HEIGHT
    };
    // Define amount to scale up the render texture
    static constexpr int8_t  s_RENDER_TEXTURE_SCALE { 4 };

    int8_t                                m_indexOfPlayer;
    RenderTexture2D                       m_target;
    AssetManager                          m_assman;   // Hehe
    std::vector<std::unique_ptr<Bullet>>  m_bullets;
    std::vector<std::unique_ptr<Entity>>  m_entities;
    std::unique_ptr<Map>                  m_map;

    void m_cleanup();
    void m_draw();
    void m_init();
    void m_restart();
    void m_tick();
    void m_ySortEntities();
};
