#include "MapState.h"

#include "GameData.h"

namespace xy {
  void MapState::initialize(const GameData& data) {
    const gf::Vector2i mapSize = data.map.level.getSize();

    fieldOfView = std::move(gf::SquareMap(mapSize));

    for (int col = 0; col < mapSize.width; ++col) {
      for (int row = 0; row < mapSize.height; ++row) {
        const MapData::Cell& cell = data.map.level(gf::vec(col, row));
        switch (cell.type) {
        case MapData::CellType::Floor:
          fieldOfView.setEmpty(gf::vec(col, row));
          break;

        default:
          break;
        }
      }
    }
  }
}
