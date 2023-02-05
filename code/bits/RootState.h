#ifndef RC_ROOT_STATE_H
#define RC_ROOT_STATE_H

#include <vector>

#include <gf/Vector.h>

namespace rc {

  struct RootState {
    gf::Vector2i tail;
    gf::Vector2i head;

    std::vector<gf::Vector2i> parts;
  };

}

#endif // RC_ROOT_STATE_H
