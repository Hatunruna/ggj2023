#include "HeroEntity.h"

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "GameData.h"
#include "GameState.h"
#include "Settings.h"

namespace xy {

  HeroEntity::HeroEntity(const GameData& data, const GameState& state, Hero hero)
  : m_data(data)
  , m_state(state)
  , m_hero(hero)
  {
  }

  void HeroEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::CircleShape hero(16.0f);

    switch (m_hero) {
      case Hero::Lisa:
        hero.setColor(gf::Color::Green);
        hero.setPosition(m_state.lisa.position * CellSize + CellSize / 2);
        break;
      case Hero::Ryan:
        hero.setColor(gf::Color::Blue);
        hero.setPosition(m_state.ryan.position * CellSize + CellSize / 2);
        break;
    }

    hero.setAnchor(gf::Anchor::Center);
    target.draw(hero, states);
  }

}
