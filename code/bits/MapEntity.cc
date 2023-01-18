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

    for (auto position : m_data.map.level.getPositionRange()) {
      const MapData::Cell& cell = m_data.map.level(position);

      gf::Color4f color;
      switch (cell.type) {
      case MapData::CellType::Floor:
        color = gf::Color::White;
        break;

      case MapData::CellType::Wall:
        color = gf::Color::Blue;
        break;
      }

      if (!mapState.fieldOfView.isInFieldOfVision(position))
      {
        if (!mapState.fieldOfView.isExplored(position)) {
          color = gf::Color::Black;
        } else if (mapState.fieldOfView.isExplored(position)) {
          color = gf::Color::darker(color, 0.7f);
        }
      }

      rectangles.addRectangle(position * CellSize, CellSize, color);
    }

    target.draw(rectangles, states);
  }

}
