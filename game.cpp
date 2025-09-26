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
Game::Game() {
  std::cout << "[Game]: created.\n";
  
  InitWindow(
    s_RENDER_TEXTURE_WIDTH*s_RENDER_TEXTURE_SCALE,
    s_RENDER_TEXTURE_HEIGHT*s_RENDER_TEXTURE_SCALE,
    "Wahoo"
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

    // Sort entities by Y position.
    m_ySortEntities();

    m_draw();
  }
}


/*
 * Unload everything.
 */
void Game::m_cleanup() {
  m_bullets.clear();
  m_entities.clear();
  m_assman.clearTextures();
}


/*
 * Draw level and entities.
 */
void Game::m_draw() {
  // First draw game to a texture so that it can be scaled up
  BeginTextureMode(m_target);
    ClearBackground(WHITE);

    m_map->draw();

    for(std::unique_ptr<Entity>& e : m_entities) {
      e->draw();
    }
    for(std::unique_ptr<Bullet>& b : m_bullets) {
      b->draw(m_assman.requestTexture(Assets::BULLET));
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
  m_map.reset(new Map(m_assman));

  m_entities.push_back(std::make_unique<Player>(
    64,
    64,
    m_assman.requestTexture(Assets::PLAYER_IDLE),
    m_assman.requestTexture(Assets::PLAYER_WALK),
    m_bullets
  ));
  m_indexOfPlayer = 0;
  m_entities.push_back(std::make_unique<Enemy>(
    16,
    80,
    m_assman.requestTexture(Assets::ZOMBIE_IDLE),
    m_assman.requestTexture(Assets::ZOMBIE_WALK)
  ));
  m_entities.push_back(std::make_unique<Enemy>(
    96,
    16,
    m_assman.requestTexture(Assets::ZOMBIE_IDLE),
    m_assman.requestTexture(Assets::ZOMBIE_WALK)
  ));
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
  for(int8_t i = 0; i < m_entities.size(); ++i) {
    const std::unique_ptr<Entity>& e { m_entities[i] };
    const int8_t indexOfSelf = i;

    if(e->isAlive) {
      e->tick(m_entities, *m_map, m_indexOfPlayer, indexOfSelf);
    }else if(indexOfSelf == m_indexOfPlayer) {
      m_restart();
    }
  }

  for(int8_t i = 0; i < m_bullets.size(); ++i) {
    const std::unique_ptr<Bullet>& b { m_bullets[i] };

    if(!b->isDestroyed) {
      b->moveAndCollide(m_entities, *m_map, m_indexOfPlayer);
    }else {
      m_bullets.erase(m_bullets.begin() + i);
    }
  }
}


/*
 * Sort entities in ascending order based on Y position.
 * Also update m_indexOfPlayer as the player is moved around.
 */
void Game::m_ySortEntities() {
  if(m_entities.size() <= 1) return;

  bool isSorted { false };
  while(!isSorted) {
    isSorted = true;
    for(int8_t i = 1; i < m_entities.size(); ++i) {
      const std::unique_ptr<Entity>& a { m_entities[i-1] };
      const std::unique_ptr<Entity>& b { m_entities[i] };

      if(a->getY() > b->getY()) {
        isSorted = false;

        std::unique_ptr<Entity> temp = std::move(m_entities[i-1]);
        m_entities[i-1] = std::move(m_entities[i]);
        m_entities[i] = std::move(temp);

        if     ((i-1) == m_indexOfPlayer) ++m_indexOfPlayer;
        else if( i    == m_indexOfPlayer) --m_indexOfPlayer;
      } 
    }  
  }
}
