#include "LightEntity.h"

#include <gf/Coordinates.h>
#include <gf/Color.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include <gf/VectorOps.h>

#include "GameState.h"

namespace rc {

  LightEntity::LightEntity(GameState& state)
  : m_state(state)
  {
  }

  void LightEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    if (m_state.phase == GamePhase::Light) {
      return;
    }

    gf::Coordinates coords(target);

    gf::RectangleShape screen(coords.getWindowSize());
    screen.setColor(gf::Color::Gray(0.1f) * gf::Color::Opaque(0.9f));
    target.draw(screen, states);
  }

}
