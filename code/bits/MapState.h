#ifndef RC_MAP_STATE_H
#define RC_MAP_STATE_H

#include <tuple>
#include <vector>

#include <gf/Array2D.h>
#include <gf/Map.h>
#include <gf/TileLayer.h>
#include <gf/Random.h>

namespace rc {

  enum class MapCellType {
    Void,
    Wall,
    Floor,
    StairUp,
    StairDown,
    Computer,
    Door,
    LiftL,
    LiftR,
  };

  inline bool isWall(MapCellType cellType) {
    return cellType == MapCellType::Wall || cellType == MapCellType::Void;
  };

  struct MapCell {
    MapCellType type;

    union {
      struct {
        bool isOpen;
      } doorState;

      struct {
        gf::Vector2i controlledDoor;
      } computerState;
    };
  };

  std::vector<gf::Array2D<MapCell>> createProceduralMap(gf::Random& random);
  std::tuple<gf::Vector2i, gf::Vector2i> computeStartingPositions(const gf::Array2D<MapCell>& cells);
  gf::SquareMap computeMap(const gf::Array2D<MapCell>& cells);
  std::vector<gf::Vector2i> computeMultiPath(const gf::SquareMap& map, std::vector<gf::Vector2i> starts, gf::Random& random);

  struct MapLevel {
    MapLevel(const gf::Array2D<MapCell>& generated, gf::Random& random);

    gf::SquareMap map;
    gf::Array2D<MapCell> cells;

    void computeDoorsAndComputers(const std::vector<gf::Vector2i>& path);
  };

  struct MapState {
    std::vector<MapLevel> levels;
    std::vector<gf::TileLayer> layers;

    void createNewMap(gf::Random& random);
  };
}

#endif // RC_MAP_STATE_H
