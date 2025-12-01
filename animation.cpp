#include "animation.h"

#include <cmath>

#include "globals.h"
#include "utils.h"


Animation::Animation(const char *spriteSheetPath)
: m_SPRITE_SHEET_PATH(spriteSheetPath)
{
  m_frameWidth  = Globals::TILE_WIDTH;
  m_frameHeight = Globals::TILE_HEIGHT;
  m_cols        = 4;
  m_rows        = 1;
  m_totalFrames = m_cols * m_rows;

  m_currentFrame    = 0;
  m_lastFrameTimeMS = getCurrentTimeMS();
}


/*
 * Reset the animation so it can start again.
 */
void Animation::restart() {
  m_currentFrame    = 0;
  m_lastFrameTimeMS = getCurrentTimeMS();
}


/*
 * Draw the current animation frame at a given position.
 * (origin is top-left corner of the image)
 */
void Animation::drawCurrentFrameAt(
  float x,
  float y,
  const std::unique_ptr<AssetManager>& assetManager
) {
  const Texture2D spriteSheet = assetManager->requestTexture(m_SPRITE_SHEET_PATH);

  // Update the index of the current frame
  m_updateCurrentFrame();

  // Get data required to draw the texture
  float w { static_cast<float>(m_frameWidth) };
  float h { static_cast<float>(m_frameHeight) };
  Rectangle pos = {x, y, w, h};
  Rectangle spriteRect = m_getRectForFrame(m_currentFrame);

  // Draw the current frame
  DrawTexturePro(spriteSheet, spriteRect, pos, (Vector2) {0, 0}, 0, WHITE);
}


/*
 * Get the rectangle that defines a given frame's position in the spritesheet.
 */
Rectangle Animation::m_getRectForFrame(uint8_t frameIndex) {
  // Get the x and y offset of the current frame in the spritesheet
  float xOffset { static_cast<float>((frameIndex % m_cols) * m_frameWidth) };
  float yOffset { static_cast<float>(((frameIndex / m_cols) + 1) * m_frameHeight) };

  // Return a new rectangle that defines the frame's position in the
  // spritesheet.
  return {
    xOffset,
    yOffset,
    static_cast<float>(m_frameWidth),
    static_cast<float>(m_frameHeight)
  };
}


/*
 * Update the index of the current frame based on the time elapsed since the
 * last update.
 */
void Animation::m_updateCurrentFrame() {
  // Get the time difference since the last update
  const double currTimeMS { getCurrentTimeMS() };
  const double timeDiffMS { currTimeMS - m_lastFrameTimeMS };

  // Update current frame index based on time difference
  if (timeDiffMS >= s_timeBetweenFramesMS) {
    uint8_t frameDiff {
      static_cast<uint8_t>(
        timeDiffMS / s_timeBetweenFramesMS
      )
    };
    m_currentFrame = (m_currentFrame + frameDiff) % m_totalFrames;

    // Update the last frame time
    // Because this method is not called at the exact same rate as the
    // animation's frame rate there will often be some "extra" time between
    // when the current frame should have been updated and the current time.
    const double extraTimeMS {
      std::fmod(
        timeDiffMS,
        s_timeBetweenFramesMS
      )
    };
    // The last frame time should be the current time minus this "extra" time
    m_lastFrameTimeMS = currTimeMS - extraTimeMS;
  }
}
