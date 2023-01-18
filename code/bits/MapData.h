#ifndef XY_MAP_DATA_H
#define XY_MAP_DATA_H

#include <gf/Array2D.h>
#include <gf/Random.h>

namespace xy {

  struct MapData {
    enum class CellType {
      Wall,
      Floor,
    };

    struct Cell {
      CellType type;
    };

    gf::Array2D<Cell> level;

    void createNewMap(gf::Random& random);
  };

}

#endif // XY_MAP_DATA_H
