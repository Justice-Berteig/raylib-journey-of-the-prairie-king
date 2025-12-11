/*
 * Class for managing how an entity is displayed.
 */

#pragma once

#include <memory>

#include "assets/animation.h"
#include "assets/asset_manager.h"


class EntityDisplay {
  public:
    EntityDisplay(
      const char *idleSpritePath,
      const char *walkSheetPath = nullptr,
      const char *deathSheetPath = nullptr
    );

    void draw(
      const std::unique_ptr<AssetManager>& assetManager,
      const bool                           isDying,
      const bool                           isMoving,
      const float                          x,
      const float                          y
    ) const;
    bool isReadyToDie(
      const bool isDying
    ) const;
    void startDeathAnim();

  private:
    const char *m_IDLE_SPRITE_PATH;
    const char *m_WALK_SHEET_PATH;
    const char *m_DEATH_SHEET_PATH;

    std::unique_ptr<Animation> m_walkAnim;
    std::unique_ptr<Animation> m_deathAnim;
};
