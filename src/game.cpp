#include "game.h"

#include <iostream>

#include "utils.h"


/*
 * Constructor initializes the window + any needed objects
 */
Game::Game()
: m_assetManager(std::make_unique<AssetManager>())
, m_entityManager()
, m_prevFrameTime(getCurrentTimeMS())
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

  SetWindowState(FLAG_VSYNC_HINT);

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
    m_entityManager.draw(m_assetManager);
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
  m_entityManager.init();
  m_map.reset(new Map(m_assetManager));
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
  // Get time since last frame and update previous frame time
  const double currentTime = getCurrentTimeMS();
  const double deltaTime = (currentTime - m_prevFrameTime) / 16.66667f;
  m_prevFrameTime = currentTime;

  // Do frame tick for entities
  m_entityManager.tick(deltaTime, m_map);

  // TODO: Send restart as callback to entity manager
  if(!m_entityManager.isPlayerAlive()) {
    m_restart();
  }
}
