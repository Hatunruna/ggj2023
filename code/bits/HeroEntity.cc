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
    const HeroState* currentState = nullptr;
    const HeroState* otherState = nullptr;
    const gf::SquareMap* currentFov = nullptr;

    switch (m_hero) {
      case Hero::Lisa:
        currentColor = gf::Color::Green;
        otherColor = gf::Color::Blue;
        currentState = &m_state.lisa;
        otherState = &m_state.ryan;
        currentFov = &m_state.lisaMap.levelsFov[currentState->levelIndex];
        break;
      case Hero::Ryan:
        currentColor = gf::Color::Blue;
        otherColor = gf::Color::Green;
        currentState = &m_state.ryan;
        otherState = &m_state.lisa;
        currentFov = &m_state.ryanMap.levelsFov[currentState->levelIndex];
        break;
    }

    assert(currentState != nullptr && otherState != nullptr && currentFov != nullptr);

    auto renderHero = [&hero, &target, &states](const HeroState* heroState, const gf::Color4f& color) {
      hero.setColor(color);
      hero.setPosition(heroState->position * CellSize + CellSize / 2);
      hero.setAnchor(gf::Anchor::Center);
      target.draw(hero, states);
    };

    // Render current
    renderHero(currentState, currentColor);

    // Render other only on the same level
    if (currentState->levelIndex == otherState->levelIndex && currentFov->isInFieldOfVision(otherState->position)) {
      renderHero(otherState, otherColor);
    }
  }

}
