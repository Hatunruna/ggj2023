#ifndef RC_SETTINGS_H
#define RC_SETTINGS_H

#include <gf/Vector.h>

namespace rc {

  inline constexpr gf::Vector2f CellSize = gf::vec(64.0f, 64.0f);
  inline constexpr gf::Vector2i SpriteSize = gf::vec(256, 256);

  inline constexpr int SpaceWidth = 21;
  inline constexpr int CorridorWidth = 2;

  inline constexpr int MapLength = SpaceWidth * 3 + CorridorWidth * 2 + 2;
  inline constexpr gf::Vector2i MapSize = gf::vec(MapLength, MapLength);

  inline constexpr int Levels = 10;
}

#endif // RC_SETTINGS_H
