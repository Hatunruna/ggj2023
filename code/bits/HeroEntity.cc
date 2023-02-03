#include "HeroEntity.h"

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "GameState.h"
#include "Settings.h"

namespace xy {

  HeroEntity::HeroEntity(const GameState& state, Hero hero)
  : m_state(state)
  , m_hero(hero)
  {
  }

  void HeroEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::CircleShape hero(16.0f);
    gf::Color4f localColor;
    gf::Color4f otherColor;
    const HeroState& localState = m_state.localPlayer(m_hero).hero;
    const HeroState& otherState = m_state.otherPlayer(m_hero).hero;
    const gf::SquareMap& localLevel = m_state.localPlayer(m_hero).map.levels[localState.levelIndex].map;

    switch (m_hero) {
      case Hero::Lisa:
        localColor = gf::Color::Green;
        otherColor = gf::Color::Blue;
        break;
      case Hero::Ryan:
        localColor = gf::Color::Blue;
        otherColor = gf::Color::Green;
        break;
    }

    auto renderHero = [&hero, &target, &states](const HeroState& heroState, const gf::Color4f& color) {
      hero.setColor(color);
      hero.setPosition(heroState.position * CellSize + CellSize / 2);
      hero.setAnchor(gf::Anchor::Center);
      target.draw(hero, states);
    };

    // Render current
    renderHero(localState, localColor);

    // Render other only on the same level
    if (localState.levelIndex == otherState.levelIndex && localLevel.isInFieldOfVision(otherState.position)) {
      renderHero(otherState, otherColor);
    }
  }

}
