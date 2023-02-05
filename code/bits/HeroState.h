#ifndef RC_HERO_STATE_H
#define RC_HERO_STATE_H

#include <gf/Vector.h>

namespace rc {

  struct HeroState {
    gf::Vector2i position;
    gf::Vector2i target;
    gf::Vector2f middle;
    int levelIndex = 0;
    bool useStairs = false;
  };

}

#endif // RC_HERO_STATE_H
