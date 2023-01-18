#include "MapData.h"

namespace xy {

  namespace {

    constexpr int MapWidth = 100;
    constexpr int MapHeight = 100;
    constexpr gf::Vector2i MapSize = gf::vec(MapWidth, MapHeight);

  }

  void MapData::createNewMap(gf::Random& random) {
    auto generateLevel = [&random](gf::Array2D<Cell>& level) {
      for (auto position : level.getPositionRange()) {
        level(position).type = static_cast<CellType>(random.computeUniformInteger(0, 1));
      }

      // Force wall on border
      for (int i = 0; i < MapWidth; ++i) {
        level(gf::vec(0, i)).type = CellType::Wall; // Top
        level(gf::vec(MapHeight - 1, i)).type = CellType::Wall; // Bottom
      }
      for (int i = 0; i < MapHeight; ++i) {
        level(gf::vec(i, 0)).type = CellType::Wall; // Left
        level(gf::vec(i, MapWidth - 1)).type = CellType::Wall; // Right
      }
    };

    levels.clear();
    for (int i = 0; i < 2; ++i) {
      levels.emplace_back(MapSize);
      generateLevel(levels[i]);
    }

    // Set first position as floor
    levels[0](gf::vec(1, 1)).type = CellType::Floor;
    levels[0](gf::vec(3, 1)).type = CellType::Floor;
  }

}
