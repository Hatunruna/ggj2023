#ifndef XY_HERO_STATE_H
#define XY_HERO_STATE_H

#include <gf/Vector.h>

namespace xy {

  struct HeroState {
    gf::Vector2i position;
    int levelIndex = 0;
  };

}

#endif // XY_HERO_STATE_H
