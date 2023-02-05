#ifndef RC_ROOT_STATE_H
#define RC_ROOT_STATE_H

#include <vector>

#include <gf/Vector.h>

namespace rc {

  struct RootPart {
    gf::Vector2i position;
    gf::Vector2i tile;
  };

  struct RootState {
    gf::Vector2i tail;
    gf::Vector2i head;

    std::vector<RootPart> parts;
  };

}

#endif // RC_ROOT_STATE_H
