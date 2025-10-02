#include "game.h"

#include <algorithm>
#include <iostream>

#include "animation.h"
#include "assets.h"
#include "enemy.h"
#include "player.h"
#include "utils.h"


/*
 * Constructor initializes the window + any needed objects
 */
Game::Game()
: m_assetManager(std::make_shared<AssetManager>()),
  m_entityManager(m_assetManager, m_map->getValidEnemySpawnPositions())
{
  std::cout << "[Game]: created.\n";
  
  InitWindow(
    s_RENDER_TEXTURE_WIDTH  * s_RENDER_TEXTURE_SCALE,
    s_RENDER_TEXTURE_HEIGHT * s_RENDER_TEXTURE_SCALE,
    "Raylib Prairie King"
  );

  m_target = LoadRenderTexture(
    s_RENDER_TEXTURE_WIDTH,
    s_RENDER_TEXTURE_HEIGHT
  );
  SetTextureFilter(m_target.texture, TEXTURE_FILTER_POINT);

  SetTargetFPS(60);

  m_init();
}


/*
 * Destructor unloads any remaining assets.
 */
Game::~Game() {
  m_cleanup();

  UnloadRenderTexture(m_target);

  CloseWindow();

  std::cout << "[Game]: destroyed.\n";
}


/*
 * Run the game.
 * Handles init/cleanup + main game loop while running.
 */
void Game::run() {
  // Main game loop
  while (!WindowShouldClose())
  {
    m_tick();
    m_draw();
  }
}


/*
 * Unload everything.
 */
void Game::m_cleanup() {
  m_bullets.clear();
  m_entityManager.cleanup();
  m_assetManager->clearTextures();
}


/*
 * Draw level and entities.
 */
void Game::m_draw() {
  // First draw game to a texture so that it can be scaled up
  BeginTextureMode(m_target);
    ClearBackground(WHITE);

    m_map->draw();
    m_entityManager.draw();

    for(std::unique_ptr<Bullet>& b : m_bullets) {
      b->draw(m_assetManager->requestTexture(Assets::BULLET));
    }
  EndTextureMode();
  // Scale up and draw the texture
  BeginDrawing();
    ClearBackground(BLACK);

    DrawTexturePro(
      m_target.texture,
      (Rectangle) {
        0.0f,
        0.0f,
        (float)m_target.texture.width,
        (float)-m_target.texture.height
      },
      (Rectangle) {
        0.0f,
        0.0f,
        (float)m_target.texture.width*s_RENDER_TEXTURE_SCALE,
        (float)m_target.texture.height*s_RENDER_TEXTURE_SCALE
      },
      (Vector2) { 0, 0 },
      0.0f,
      WHITE
    );
  EndDrawing();
}


/*
 * Initialize the game.
 */
void Game::m_init() {
  m_map.reset(new Map(m_assetManager));

  m_entityManager.addEntity(std::make_unique<Player>(
    64,
    64,
    m_assetManager->requestTexture(Assets::PLAYER_IDLE),
    m_assetManager->requestTexture(Assets::PLAYER_WALK),
    m_bullets
  ));
  m_entityManager.isPlayerAlive = true;
}


/*
 * Restart the game.
 */
void Game::m_restart() {
  std::cout << "[Game]: restarting...\n";

  m_cleanup();
  m_init();
}


/*
 * Process game tick for entities and bullets.
 */
void Game::m_tick() {
  m_entityManager.tick(m_map);

  for(int8_t i = 0; i < m_bullets.size(); ++i) {
    const std::unique_ptr<Bullet>& b { m_bullets[i] };

    if(!b->isDestroyed) {
      b->moveAndCollide(m_entityManager.getEntities(), *m_map, m_entityManager.indexOfPlayer);
    }else {
      m_bullets.erase(m_bullets.begin() + i);
    }
  }

  if(!m_entityManager.isPlayerAlive) {
    m_restart();
  }
}
