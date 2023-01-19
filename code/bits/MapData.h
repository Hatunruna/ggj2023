#ifndef XY_MAP_DATA_H
#define XY_MAP_DATA_H

#include <gf/Array2D.h>
#include <gf/Random.h>

namespace xy {

  struct MapData {
    enum class CellType {
      Wall,
      Floor,
      StairUp,
      StairDown,
    };

    struct Cell {
      CellType type;
    };

    std::vector<gf::Array2D<Cell>> levels;

    void createNewMap(gf::Random& random);
  };

}

#endif // XY_MAP_DATA_H
