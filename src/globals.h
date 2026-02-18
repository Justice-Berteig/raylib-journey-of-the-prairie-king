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
  inline constexpr int8_t ZOMBIE_STARTING_HEALTH  { 1 };
  inline constexpr int8_t BULLET_STARTING_HEALTH { 1 };
}
