#include "utils.h"

#include <chrono>


/*
 * Draws a given texture at a given location.
 * Basically just a wrapper for DrawTexturePro()
 * so that I don't have to manually make the rectangles.
 */
void drawTextureAt(
  const Texture2D &tex,
  const float x,
  const float y
) {
  // Get the width and height of the texture
  const float w { static_cast<float>(tex.width) };
  const float h { static_cast<float>(tex.height) };

  // Create rectangles for drawing the texture
  Rectangle pos {x, y, w, h};
  Rectangle rec {0, 0, w, h};

  // Use raylib's DrawTexturePro to actually draw the texture
  DrawTexturePro(tex, rec, pos, (Vector2) {0, 0}, 0, WHITE);
}


/*
 * Uses std::chrono to get the current system time in milliseconds.
 * The code for doing this was too verbose so I made a function for it.
 */
double getCurrentTimeMS() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(
    system_clock::now().time_since_epoch()
  ).count();
}
