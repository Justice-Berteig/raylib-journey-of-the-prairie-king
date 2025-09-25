/*
 * Class for displaying and checking collisions with
 * a map in the game.
 */

#pragma once

#include <cstdint>

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
    Map(AssetManager& assman);

    bool isCollidingWith(const Rectangle& rect) const;
    void draw() const;

  private:
    Texture2D m_floorTexture;
    Texture2D m_obstacleTexture;
    Texture2D m_wallTexture;
    TileType m_tiles[Globals::MAP_WIDTH][Globals::MAP_HEIGHT];

    void m_generate();
};
