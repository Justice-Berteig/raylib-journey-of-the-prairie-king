#include "asset_manager.h"

#include <cstring>
#include <iostream>


AssetManager::AssetManager() {
  std::cout << "[AssetManager]: created.\n";
}


AssetManager::~AssetManager() {
  clearTextures();
  std::cout << "[AssetManager]: destroyed.\n";
}


/*
 * Load and store a texture from a given path.
 */
void AssetManager::loadTexture(const char *path) {
  std::cout << "[AssetManager]: Loading texture at " << path << '\n';
  m_textures.emplace(path, LoadTexture(path));
}


/*
 * Check if a given texture has already been loaded,
 * load it if it has not been loaded, and return the texture.
 */
Texture2D& AssetManager::requestTexture(const char *path) {
  if(!m_textures.contains(path)) {
    loadTexture(path);
  }
  return m_textures[path];
}


/*
 * Unload all loaded textures.
 */
void AssetManager::clearTextures() {
  std::cout << "[AssetManager]: Clear textures\n";
  for(const auto& [key, value] : m_textures) {
    std::cout << "Freeing " << key << '\n';
    UnloadTexture(value);
  }
  m_textures.clear();
}
