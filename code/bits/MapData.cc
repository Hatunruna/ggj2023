#include "MapData.h"

namespace xy {

  namespace {

    constexpr gf::Vector2i MapSize = gf::vec(100, 100);

  }

  void MapData::createNewMap(gf::Random& random) {
    level = gf::Array2D<Cell>(MapSize);

    for (auto position : level.getPositionRange()) {
      level(position).color = random.computeUniformInteger(0, 7);
    }
  }

}
