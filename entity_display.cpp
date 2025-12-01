#include "entity_display.h"

#include "utils.h"


EntityDisplay::EntityDisplay(
  const char *idleSpritePath,
  const char *walkSheetPath
)
: m_IDLE_SPRITE_PATH(idleSpritePath)
, m_WALK_SHEET_PATH(walkSheetPath)
{
  if(walkSheetPath) m_walkAnim.reset(new Animation(m_WALK_SHEET_PATH));
  else m_walkAnim = nullptr;
}


void EntityDisplay::draw(
  const std::shared_ptr<AssetManager>& assetManager,
  const bool isMoving,
  const float x,
  const float y
) const {
  if(isMoving && m_walkAnim) {
    m_walkAnim->drawCurrentFrameAt(
      x,
      y,
      assetManager
    );
  }else {
    drawTextureAt(
      assetManager->requestTexture(m_IDLE_SPRITE_PATH),
      x,
      y 
    );
  }
}
