#include "MapEntity.h"

#include <gf/Particles.h>
#include <gf/Sprite.h>
#include <gf/RenderTarget.h>

#include "GameHub.h"
#include "GameState.h"
#include "Settings.h"

namespace rc {

  MapEntity::MapEntity(GameHub& game, GameState& state, Hero hero)
  : m_state(state)
  , m_hero(hero)
  {
  }

  void MapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
//     if (m_state.phase == GamePhase::Dark) {
//       return;
//     }

    PlayerState& playerState = m_state.localPlayer(m_hero);
    target.draw(playerState.map.layers[playerState.hero.levelIndex], states);

    gf::ShapeParticles rectangles;

    const MapState& mapState = m_state.localPlayer(m_hero).map;
    const int levelIndex = m_state.localPlayer(m_hero).hero.levelIndex;
    const gf::Array2D<MapCell>& level = mapState.levels[levelIndex].level.cells;
    const gf::SquareMap& fov = mapState.levels[levelIndex].map;

    for (auto position : level.getPositionRange()) {
      const MapCell& cell = level(position);

      gf::Color4f color;
      if (!fov.isInFieldOfVision(position))
      {
        if (!fov.isExplored(position)) {
          rectangles.addRectangle(position * CellSize, CellSize, gf::Color::Black);
        } else if (fov.isExplored(position)) {
          rectangles.addRectangle(position * CellSize, CellSize, gf::Color::Black * gf::Color::Opaque(0.7f));
        }
      }
    }

    target.draw(rectangles, states);
  }

}
