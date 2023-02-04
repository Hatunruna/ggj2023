#ifndef XY_ROOT_STATE_H
#define XY_ROOT_STATE_H

#include <vector>

#include <gf/Vector.h>

namespace xy {

  struct RootState {
    gf::Vector2i tail;
    gf::Vector2i head;

    std::vector<gf::Vector2i> parts;
  };

}

#endif // XY_ROOT_STATE_H
