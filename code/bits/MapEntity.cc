#include "MapEntity.h"

#include <gf/Particles.h>
#include <gf/RenderTarget.h>

#include "GameData.h"
#include "GameState.h"
#include "Settings.h"

namespace xy {

  MapEntity::MapEntity(const GameData& data, const GameState& state, Hero hero)
  : m_data(data)
  , m_state(state)
  , m_hero(hero)
  {
  }

  void MapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::ShapeParticles rectangles;

    const MapState& mapState = m_state.maps[static_cast<int>(m_hero)];
    const int levelIndex = m_state.heros[static_cast<int>(m_hero)].levelIndex;
    const gf::Array2D<MapData::Cell>& level = m_data.map.levels[levelIndex];
    const gf::SquareMap& fov = m_state.maps[static_cast<int>(m_hero)].levelsFov[levelIndex];

    for (auto position : level.getPositionRange()) {
      const MapData::Cell& cell = level(position);

      gf::Color4f color;
      switch (cell.type) {
      case MapData::CellType::Floor:
        color = gf::Color::White;
        break;

      case MapData::CellType::Wall:
        color = gf::Color::Blue;
        break;

      case MapData::CellType::StairDown:
        color = gf::Color::Yellow;
        break;

      case MapData::CellType::StairUp:
        color = gf::Color::Magenta;
        break;
      }

      if (!fov.isInFieldOfVision(position))
      {
        if (!fov.isExplored(position)) {
          color = gf::Color::Black;
        } else if (fov.isExplored(position)) {
          color = gf::Color::darker(color, 0.7f);
        }
      }

      rectangles.addRectangle(position * CellSize, CellSize, color);
    }

    target.draw(rectangles, states);
  }

}
