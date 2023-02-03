#include "MapEntity.h"

#include <gf/Particles.h>
#include <gf/RenderTarget.h>

#include "GameState.h"
#include "Settings.h"

namespace xy {

  MapEntity::MapEntity(const GameState& state, Hero hero)
  : m_state(state)
  , m_hero(hero)
  {
  }

  void MapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::ShapeParticles rectangles;

    const int index = static_cast<int>(m_hero);
    const MapState& mapState = m_state.maps[index];
    const int levelIndex = m_state.heros[index].levelIndex;
    const gf::Array2D<MapCell>& level = m_state.maps[index].levels[levelIndex].cells;
    const gf::SquareMap& fov = m_state.maps[index].levels[levelIndex].map;

    for (auto position : level.getPositionRange()) {
      const MapCell& cell = level(position);

      gf::Color4f color;
      switch (cell.type) {
      case MapCellType::Floor:
        color = gf::Color::White;
        break;

      case MapCellType::Wall:
        color = gf::Color::Blue;
        break;

      case MapCellType::StairDown:
        color = gf::Color::Yellow;
        break;

      case MapCellType::StairUp:
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
