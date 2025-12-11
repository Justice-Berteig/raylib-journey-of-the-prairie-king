#include "entity_display.h"

#include "utils.h"


EntityDisplay::EntityDisplay(
  const char *idleSpritePath,
  const char *walkSheetPath,
  const char *deathSheetPath
)
: m_IDLE_SPRITE_PATH(idleSpritePath)
, m_WALK_SHEET_PATH(walkSheetPath)
, m_DEATH_SHEET_PATH(deathSheetPath)
, m_walkAnim(nullptr)
, m_deathAnim(nullptr)
{
  if(m_WALK_SHEET_PATH) m_walkAnim.reset(new Animation(m_WALK_SHEET_PATH));
}


void EntityDisplay::draw(
  const std::unique_ptr<AssetManager>& assetManager,
  const bool isDying,
  const bool isMoving,
  const float x,
  const float y
) const {
  if(isDying) {
    if(m_deathAnim) {
      m_deathAnim->drawCurrentFrameAt(
        x,
        y,
        assetManager
      );
    } else return;
  } else if(isMoving && m_walkAnim) {
    m_walkAnim->drawCurrentFrameAt(
      x,
      y,
      assetManager
    );
  } else {
    drawTextureAt(
      assetManager->requestTexture(m_IDLE_SPRITE_PATH),
      x,
      y 
    );
  }
}


bool EntityDisplay::isReadyToDie(
  const bool isDying
) const {
  if(isDying) {
    if(!m_deathAnim) return true;
    else if(m_deathAnim->isFinished()) return true;
  }
  return false;
}


void EntityDisplay::startDeathAnim() {
  if(m_DEATH_SHEET_PATH) m_deathAnim.reset(new Animation(m_DEATH_SHEET_PATH, false));
}
