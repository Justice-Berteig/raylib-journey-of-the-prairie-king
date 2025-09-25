/*
 * Helper class for loading, storing, distributing, and unloading
 * assets such as textures and sounds.
 */

#pragma once

#include <map>
#include <string>

#include "raylib.h"


class AssetManager {
  public:
    AssetManager();
    ~AssetManager();

    void      clearTextures();
    void      loadTexture(const char *path);
    Texture2D requestTexture(const char *path);

  private:
    std::map<std::string, Texture2D> m_textures;
};
