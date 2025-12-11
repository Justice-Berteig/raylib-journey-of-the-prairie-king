#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "raylib.h"

#include "assets/asset_manager.h"
#include "entities/entity_manager.h"
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

    std::unique_ptr<AssetManager> m_assetManager;
    EntityManager                 m_entityManager;
    std::unique_ptr<Map>          m_map;
    RenderTexture2D               m_target;

    void m_cleanup();
    void m_draw();
    void m_init();
    void m_restart();
    void m_tick();
};
