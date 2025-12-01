/*
 * Class for managing how an entity is displayed.
 */

#pragma once

#include <memory>

#include "animation.h"
#include "asset_manager.h"

class EntityDisplay {
  public:
    EntityDisplay(
      const char *idleSpritePath,
      const char *walkSheetPath
    );

    void draw(
      const std::unique_ptr<AssetManager>& assetManager,
      const bool isMoving,
      const float x,
      const float y
    ) const;

  private:
    const char *m_IDLE_SPRITE_PATH;
    const char *m_WALK_SHEET_PATH;

    std::unique_ptr<Animation> m_walkAnim;
};
