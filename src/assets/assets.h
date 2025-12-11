/*
 * Define paths for assets.
 */

#pragma once

#include <string>

namespace Assets {
  // Define paths for player textures
  inline constexpr const char *PLAYER_IDLE {"../resources/player_idle.png"};
  inline constexpr const char *PLAYER_WALK {"../resources/player_walk.png"};

  // Define paths for enemy textures
  inline constexpr const char *ZOMBIE_IDLE  {"../resources/zombie_idle.png"};
  inline constexpr const char *ZOMBIE_WALK  {"../resources/zombie_walk.png"};
  inline constexpr const char *ZOMBIE_DEATH {"../resources/zombie_death.png"};

  // Define paths for bullet textures
  inline constexpr const char *BULLET {"../resources/bullet.png"};

  // Define paths for map textures
  inline constexpr const char *GRASS_FLOOR    {"../resources/grass_floor.png"};
  inline constexpr const char *GRASS_OBSTACLE {"../resources/grass_obstacle.png"};
  inline constexpr const char *GRASS_WALL     {"../resources/grass_wall.png"};
}
