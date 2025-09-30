/*
 * Class for animating a spritesheet.
 */

#pragma once

#include <cstdint>

#include "raylib.h"


class Animation {
  public:
    Animation(Texture2D spriteSheet);

    void drawCurrentFrameAt(float x, float y);
    void restart();

  private:
    static constexpr uint8_t s_targetFrameRate     = 12;
    static constexpr double  s_timeBetweenFramesMS = 1000 / s_targetFrameRate;

    uint8_t   m_cols;
    uint8_t   m_rows;
    uint8_t   m_frameWidth;
    uint8_t   m_frameHeight;
    uint8_t   m_totalFrames;
    uint8_t   m_currentFrame;
    double    m_lastFrameTimeMS;
    Texture2D m_spriteSheet;

    Rectangle m_getRectForFrame(uint8_t frameIndex);
    void      m_updateCurrentFrame();
};
