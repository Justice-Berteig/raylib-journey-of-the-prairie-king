/*
 * Generic helpful functions that don't belong anywhere else.
 */

#pragma once

#include "raylib.h"


void drawTextureAt(
  const Texture2D &tex,
  const float x,
  const float y
);


double getCurrentTimeMS();
