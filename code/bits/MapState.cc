#include "MapState.h"

namespace xy {

  namespace {

    constexpr int MapWidth = 100;
    constexpr int MapHeight = 100;
    constexpr gf::Vector2i MapSize = gf::vec(MapWidth, MapHeight);

  }

  std::vector<gf::Array2D<MapCell>> createProceduralMap(gf::Random& random) {
    auto generateLevel = [&random](gf::Vector2i mapSize) -> gf::Array2D<MapCell> {
      gf::Array2D<MapCell> level(mapSize);

      for (auto position : level.getPositionRange()) {
        MapCellType type = MapCellType::Floor;

        if (random.computeUniformInteger(0, 99) < 20) {
          type = MapCellType::Wall;
        }

        level(position).type = type;
      }

      // Force wall on border
      for (int i = 0; i < MapWidth; ++i) {
        level(gf::vec(0, i)).type = MapCellType::Wall; // Top
        level(gf::vec(MapHeight - 1, i)).type = MapCellType::Wall; // Bottom
      }

      for (int i = 0; i < MapHeight; ++i) {
        level(gf::vec(i, 0)).type = MapCellType::Wall; // Left
        level(gf::vec(i, MapWidth - 1)).type = MapCellType::Wall; // Right
      }

      return level;
    };

    std::vector<gf::Array2D<MapCell>> levels;

    for (int i = 0; i < 2; ++i) {
      levels.emplace_back(generateLevel(MapSize));
    }

    // Set first position as floor
    levels[0](gf::vec(1, 1)).type = MapCellType::Floor;
    levels[0](gf::vec(3, 1)).type = MapCellType::Floor;

    // Set stairs
    levels[0](gf::vec(1, 2)).type = MapCellType::StairUp;
    levels[1](gf::vec(1, 2)).type = MapCellType::StairDown;
    levels[0](gf::vec(3, 2)).type = MapCellType::StairUp;
    levels[1](gf::vec(3, 2)).type = MapCellType::StairDown;

    return levels;
  }

  MapLevel::MapLevel(const gf::Array2D<MapCell>& generated)
  : map(generated.getSize())
  , cells(generated)
  {
    const gf::Vector2i mapSize = generated.getSize();

    for (int col = 0; col < mapSize.width; ++col) {
      for (int row = 0; row < mapSize.height; ++row) {
        const MapCell& cell = cells(gf::vec(col, row));
        switch (cell.type) {
        case MapCellType::Floor:
        case MapCellType::StairDown:
        case MapCellType::StairUp:
          map.setEmpty(gf::vec(col, row));
          break;

        default:
          break;
        }
      }
    }

  }

}
