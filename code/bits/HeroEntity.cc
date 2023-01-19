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
    gf::Color4f currentColor;
    gf::Color4f otherColor;
    const HeroState& currentState = m_state.heros[static_cast<int>(m_hero)];
    const HeroState& otherState = m_state.heros[getOtherHeroIndex(m_hero)];
    const gf::SquareMap& currentFov = m_state.maps[static_cast<int>(m_hero)].levelsFov[currentState.levelIndex];

    switch (m_hero) {
      case Hero::Lisa:
        currentColor = gf::Color::Green;
        otherColor = gf::Color::Blue;
        break;
      case Hero::Ryan:
        currentColor = gf::Color::Blue;
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
    renderHero(currentState, currentColor);

    // Render other only on the same level
    if (currentState.levelIndex == otherState.levelIndex && currentFov.isInFieldOfVision(otherState.position)) {
      renderHero(otherState, otherColor);
    }
  }

}
