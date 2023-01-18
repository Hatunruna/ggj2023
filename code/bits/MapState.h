#ifndef XY_MAP_STATE_H
#define XY_MAP_STATE_H

#include <gf/Map.h>

namespace xy {
  class GameData;

  struct MapState {
    gf::SquareMap fieldOfView = gf::SquareMap(gf::vec(1, 1));

    void initialize(const GameData& data);
  };
}

#endif // XY_MAP_STATE_H
