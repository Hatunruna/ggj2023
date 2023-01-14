#include "MapEntity.h"

#include <gf/Particles.h>
#include <gf/RenderTarget.h>

#include "GameData.h"
#include "GameState.h"
#include "Settings.h"

namespace xy {

  constexpr gf::Color4f Colors[] = {
    gf::ColorF::Black,
    gf::ColorF::White,
    gf::ColorF::Orange,
    gf::ColorF::Rose,
    gf::ColorF::Chartreuse,
    gf::ColorF::Spring,
    gf::ColorF::Violet,
    gf::ColorF::Azure,
  };

  MapEntity::MapEntity(const GameData& data, const GameState& state, Hero hero)
  : m_data(data)
  , m_state(state)
  , m_hero(hero)
  {
  }

  void MapEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::ShapeParticles rectangles;

    for (auto position : m_data.map.level.getPositionRange()) {
      int color = m_data.map.level(position).color;
      assert(0 <= color && color < 8);
      rectangles.addRectangle(position * CellSize, CellSize, Colors[color]);
    }

    target.draw(rectangles, states);
  }

}
