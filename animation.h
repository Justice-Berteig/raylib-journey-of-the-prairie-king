/*
 * Class for animating a spritesheet.
 */

#pragma once

#include <cstdint>
#include <memory>

#include "raylib.h"

#include "asset_manager.h"


class Animation {
  public:
    Animation(const char *spriteSheetPath);

    void drawCurrentFrameAt(
      float x,
      float y,
      const std::unique_ptr<AssetManager>& assetManager
    );
    void restart();

  private:
    static constexpr uint8_t s_targetFrameRate     = 12;
    static constexpr double  s_timeBetweenFramesMS = 1000 / s_targetFrameRate;

    const char *m_SPRITE_SHEET_PATH;

    uint8_t m_cols;
    uint8_t m_rows;
    uint8_t m_frameWidth;
    uint8_t m_frameHeight;
    uint8_t m_totalFrames;
    uint8_t m_currentFrame;
    double  m_lastFrameTimeMS;

    Rectangle m_getRectForFrame(uint8_t frameIndex);
    void      m_updateCurrentFrame();
};
