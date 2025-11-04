/*
 * Define some global variables to be used throughout
 * the application.
 */

#pragma once

#include <cstdint>
#include <vector>

#include "raylib.h"

namespace Globals {
  // Define width and height of map tiles in pixels
  inline constexpr int8_t TILE_WIDTH  { 16 };
  inline constexpr int8_t TILE_HEIGHT { TILE_WIDTH };
  // Define width and height of game in tiles
  inline constexpr int8_t MAP_WIDTH  { 11 };
  inline constexpr int8_t MAP_HEIGHT { 11 };

  // Define starting health values
  inline constexpr int8_t PLAYER_STARTING_HEALTH { 1 };
  inline constexpr int8_t ENEMY_STARTING_HEALTH  { 1 };
  inline constexpr int8_t BULLET_STARTING_HEALTH { 1 };

  // Define function to get list of valid spawn points
  constexpr std::vector<Vector2> getValidEnemySpawnPositions() {
    std::vector<Vector2> positions;

    for(int8_t tileX { 0 }; tileX < MAP_WIDTH; ++tileX) {
      if(tileX == 0 || tileX == MAP_WIDTH - 1) continue;
      for(int8_t tileY { 0 }; tileY < MAP_HEIGHT; ++tileY) {
        if(tileY == 0 || tileY == MAP_HEIGHT - 1) continue;
        if(
          tileX == 1
          || tileY == 1
          || tileX == MAP_WIDTH - 2
          || tileY == MAP_HEIGHT - 2
        ) {
          positions.emplace_back(
            tileX * TILE_WIDTH,
            tileY * TILE_HEIGHT
          );
        }
      }
    }

    return positions;
  };
}
