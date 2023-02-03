#ifndef XY_MAP_STATE_H
#define XY_MAP_STATE_H

#include <gf/Array2D.h>
#include <gf/Map.h>
#include <gf/Random.h>

namespace xy {

  enum class MapCellType {
    Wall,
    Floor,
    StairUp,
    StairDown,
  };

  struct MapCell {
    MapCellType type;
  };

  std::vector<gf::Array2D<MapCell>> createProceduralMap(gf::Random& random);

  struct MapLevel {
    MapLevel(const gf::Array2D<MapCell>& generated);

    gf::SquareMap map;
    gf::Array2D<MapCell> cells;
  };

  struct MapState {
    std::vector<MapLevel> levels;

    void createNewMap(gf::Random& random);
  };
}

#endif // XY_MAP_STATE_H
