#include "map.h"

#include <cstdlib>
#include <ctime>

#include "assets.h"
#include "utils.h"


/*
 * Constructor requests needed textures
 * and does initial map generation.
 */
Map::Map(std::shared_ptr<AssetManager>& assetManager) {
  // Request textures
  m_floorTexture    = assetManager->requestTexture(Assets::GRASS_FLOOR);
  m_obstacleTexture = assetManager->requestTexture(Assets::GRASS_OBSTACLE);
  m_wallTexture     = assetManager->requestTexture(Assets::GRASS_WALL);

  // Do initial map generation
  m_generate();
}


/*
 * Check if a given rectangle is colliding with any solid part of the map.
 */
bool Map::isCollidingWith(const Rectangle& rect) const {
  // Get min and max tile indeces
  int8_t xMin {
    static_cast<int8_t>(
      rect.x / Globals::TILE_WIDTH
    )
  };
  int8_t yMin {
    static_cast<int8_t>(
      rect.y / Globals::TILE_HEIGHT
    )
  };
  int8_t xMax {
    static_cast<int8_t>(
      (rect.x + rect.width - 1) / Globals::TILE_WIDTH
    )
  };
  int8_t yMax {
    static_cast<int8_t>(
      (rect.y + rect.height - 1) / Globals::TILE_HEIGHT
    )
  };

  // Clamp tile indeces
  if(xMin < 0) xMin = 0;
  if(yMin < 0) yMin = 0;
  if(xMax >= Globals::MAP_WIDTH) xMax = Globals::MAP_WIDTH - 1;
  if(yMax >= Globals::MAP_HEIGHT) yMax = Globals::MAP_HEIGHT - 1;

  // Check if any of the intersected tiles are solid
  for(int8_t x = xMin; x <= xMax; ++x) {
    for(int8_t y = yMin; y <= yMax; ++y) {
      if(m_tiles[x][y] != TileType::FLOOR) {
        return true;
      }
    }
  }
  return false;
}


/*
 * Draw the map.
 */
void Map::draw() const {
  const Texture2D *selectedTexture { &m_floorTexture };

  // Loop for each tile in the map
  for(int8_t x = 0; x < Globals::MAP_WIDTH; ++x) {
    for(int8_t y = 0; y < Globals::MAP_HEIGHT; ++y) {

      // Get screen position where tile should be drawn
      int16_t xPos {
        static_cast<int16_t>(
          x * Globals::TILE_WIDTH
        )
      };
      int16_t yPos {
        static_cast<int16_t>(
          y * Globals::TILE_HEIGHT
        )
      };

      if(m_tiles[x][y] == TileType::WALL) {
        selectedTexture = &m_wallTexture;
      }else if(m_tiles[x][y] == TileType::OBSTACLE) {
        selectedTexture = &m_obstacleTexture;
      }else {
        selectedTexture = &m_floorTexture;
      }

      drawTextureAt(
        *selectedTexture,
        xPos,
        yPos
      );
    }
  }
}


/*
 * Generate the map.
 */
void Map::m_generate() {
  // Initialize random number generator
  srand(time(0));

  // Loop for each tile in the map
  for(int8_t x = 0; x < Globals::MAP_WIDTH; ++x) {
    for(int8_t y = 0; y < Globals::MAP_HEIGHT; ++y) {

      if(
        x == 0
        || y == 0
        || x == (Globals::MAP_WIDTH - 1)
        || y == (Globals::MAP_HEIGHT - 1)
      ) {
        // If this tile is right on the edge of the map
        m_tiles[x][y] = TileType::WALL;
      } else if(
        x == 1
        || y == 1
        || x == (Globals::MAP_WIDTH - 2)
        || y == (Globals::MAP_HEIGHT - 2)
      ) {
        // If this tile is just before the edge of the map
        m_tiles[x][y] = TileType::FLOOR;
      }else {
        // If this tile is somewhere in the middle of the map
        // 8% of the tiles should be bushes
        int randomNum { rand() % 101 };
        if(randomNum < 92) {
          m_tiles[x][y] = TileType::FLOOR;
        }else {
          m_tiles[x][y] = TileType::OBSTACLE;
        }
      }

    }
  }
}
