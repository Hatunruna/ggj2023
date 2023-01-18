#ifndef XY_MAP_STATE_H
#define XY_MAP_STATE_H

#include <gf/Map.h>

namespace xy {
  class GameData;

  struct MapState {
    std::vector<gf::SquareMap> levelsFov;

    void initialize(const GameData& data);
  };
}

#endif // XY_MAP_STATE_H
