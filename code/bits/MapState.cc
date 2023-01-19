#include "MapState.h"

#include "GameData.h"

namespace xy {
  void MapState::initialize(const GameData& data) {
    auto generateFov = [](gf::SquareMap& fov, const gf::Array2D<MapData::Cell>& level) {
      const gf::Vector2i mapSize = level.getSize();

      for (int col = 0; col < mapSize.width; ++col) {
        for (int row = 0; row < mapSize.height; ++row) {
          const MapData::Cell& cell = level(gf::vec(col, row));
          switch (cell.type) {
          case MapData::CellType::Floor:
          case MapData::CellType::StairDown:
          case MapData::CellType::StairUp:
            fov.setEmpty(gf::vec(col, row));
            break;

          default:
            break;
          }
        }
      }
    };

    const int levelsCount = data.map.levels.size();
    levelsFov.clear();
    for (int i = 0; i < levelsCount; ++i) {
      const gf::Vector2i mapSize = data.map.levels[i].getSize();
      levelsFov.emplace_back(mapSize);
      generateFov(levelsFov[i], data.map.levels[i]);
    }
  }
}
