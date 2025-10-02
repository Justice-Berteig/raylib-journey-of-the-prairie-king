/*
 * Class for displaying and checking collisions with
 * a map in the game.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "asset_manager.h"
#include "globals.h"
#include "raylib.h"


enum class TileType {
  FLOOR,
  OBSTACLE,
  WALL
};


class Map {
  public:
    Map(std::shared_ptr<AssetManager>& assetManager);

    constexpr std::vector<Vector2> getValidEnemySpawnPositions() {
      std::vector<Vector2> positions;

      for(int8_t tileX { 0 }; tileX < Globals::MAP_WIDTH; ++tileX) {
        if(tileX == 0 || tileX == Globals::MAP_WIDTH - 1) continue;
        for(int8_t tileY { 0 }; tileY < Globals::MAP_HEIGHT; ++tileY) {
          if(tileY == 0 || tileY == Globals::MAP_HEIGHT - 1) continue;
          if(
            tileX == 1
            || tileY == 1
            || tileX == Globals::MAP_WIDTH - 2
            || tileY == Globals::MAP_HEIGHT - 2
          ) {
            positions.emplace_back(
              tileX * Globals::TILE_WIDTH,
              tileY * Globals::TILE_HEIGHT
            );
          }
        }
      }

      return positions;
    };
    void                 draw() const;
    bool                 isCollidingWith(const Rectangle& rect) const;

  private:
    Texture2D m_floorTexture;
    Texture2D m_obstacleTexture;
    Texture2D m_wallTexture;
    TileType m_tiles[Globals::MAP_WIDTH][Globals::MAP_HEIGHT];

    void m_generate();
};
