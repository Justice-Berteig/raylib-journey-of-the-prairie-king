#include "map.h"

#include <cstdlib>
#include <ctime>
#include <utility>

#include "assets/assets.h"
#include "utils.h"


/*
 * Constructor requests needed textures
 * and does initial map generation.
 */
Map::Map(std::unique_ptr<AssetManager>& assetManager) {
  // Request textures
  m_floorTexture    = assetManager->requestTexture(Assets::GRASS_FLOOR);
  m_obstacleTexture = assetManager->requestTexture(Assets::GRASS_OBSTACLE);
  m_wallTexture     = assetManager->requestTexture(Assets::GRASS_WALL);

  // Do initial map generation
  m_generate();
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
 * Generate the map.
 */
void Map::m_generate() {
  // Initialize vector to store tile indeces for tiles that can have an obstacle
  std::vector<std::pair<int8_t, int8_t>> potentialObstacleTiles;
  potentialObstacleTiles.reserve(Globals::MAP_WIDTH * Globals::MAP_HEIGHT);

  // First pass to add walls/floors
  for(int8_t x = 0; x < Globals::MAP_WIDTH; ++x) {
    for(int8_t y = 0; y < Globals::MAP_HEIGHT; ++y) {
      // For each tile in the map

      if(
        x == 0
        || y == 0
        || x == (Globals::MAP_WIDTH - 1)
        || y == (Globals::MAP_HEIGHT - 1)
      ) {
        // If this tile is right on the edge of the map
        // make it a wall
        m_tiles[x][y] = TileType::WALL;
      } else {
        // Otherwise
        // make it a floor
        m_tiles[x][y] = TileType::FLOOR;

        // Check if tile could be an obstacle
        if(
          x != 1
          && y != 1
          && x != (Globals::MAP_WIDTH - 2)
          && y != (Globals::MAP_HEIGHT - 2)
          && (
            x < (Globals::MAP_WIDTH / 2) - 1
            || x > (Globals::MAP_WIDTH / 2) + 1
            || y < (Globals::MAP_HEIGHT / 2) - 1
            || y > (Globals::MAP_HEIGHT / 2) + 1
          )
        ) {
          // If this tile could be an obstacle
          // add its indeces to the list of potential obstacle tiles
          potentialObstacleTiles.push_back({x, y});
        }
      }
    }
  }

  // Second pass to add obstacles
  // Initialize random number generator
  srand(time(0));
  const int8_t numObstacles { 6 };
  for(int8_t i = 0; i < numObstacles; ++i) {
    // Get a random potential obstacle tile
    const int8_t obstacleTileIndex {
      static_cast<int8_t>(rand() % potentialObstacleTiles.size())
    };
    const auto obstacleTile { potentialObstacleTiles.at(obstacleTileIndex) };

    // make it an obstacle
    m_tiles[obstacleTile.first][obstacleTile.second] = TileType::OBSTACLE;

    // remove it from the list of potential obstacle tiles
    potentialObstacleTiles.erase(
      potentialObstacleTiles.begin() + obstacleTileIndex
    );
  }
}
