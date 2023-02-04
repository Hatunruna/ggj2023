#include "HeroEntity.h"

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "GameData.h"
#include "GameState.h"
#include "Settings.h"

namespace xy {

  namespace {

    constexpr gf::Time MoveCooldown = gf::seconds(0.25f);

  }

  HeroEntity::HeroEntity(const GameData& data, GameState& state, Hero hero)
  : m_data(data)
  , m_state(state)
  , m_hero(hero)
  , stillRuning(false)
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
      hero.setPosition(heroState.middle + CellSize / 2);
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

  void HeroEntity::update(gf::Time time) {
    m_moveCooldown += time;
    HeroState& currentState = m_state.heros[static_cast<int>(m_hero)];

    if (currentState.position == currentState.target) {
      currentState.middle = currentState.position * CellSize;
      return;
    }

    if (m_moveCooldown > MoveCooldown) {
      currentState.position = currentState.target;
      currentState.middle = currentState.position * CellSize;
    } else {
      currentState.middle = gf::lerp(currentState.position * CellSize, currentState.target * CellSize, m_moveCooldown.asSeconds() / MoveCooldown.asSeconds());
    }
  }

  void HeroEntity::move(gf::Direction dir) {
    HeroState& currentState = m_state.heros[static_cast<int>(m_hero)];
    HeroState& otherState = m_state.heros[getOtherHeroIndex(m_hero)];

    if (dir == gf::Direction::Center) {
      stillRuning = false;
      return;
    }
    stillRuning = true;

    if (m_moveCooldown < MoveCooldown) {
//       gf::Log::debug("Cooldown not yet finished!\n");
      return;
    }

    m_currentDirection = dir;

    gf::Vector2i nextPos = currentState.position + gf::displacement(dir);

    const int levelIndex = currentState.levelIndex;
    const gf::SquareMap& fov = m_state.maps[static_cast<int>(m_hero)].levelsFov[levelIndex];

    bool nextPositionAlreadyOccupied =
      (otherState.levelIndex == levelIndex)
      && (otherState.position == nextPos);
    if (nextPos == currentState.position || !fov.isWalkable(nextPos) || nextPositionAlreadyOccupied) {
//       gf::Log::debug("Collision!\n");
      return;
    }

    // stairs things
    if (m_data.map.levels[levelIndex](nextPos).type == MapData::CellType::StairDown && !currentState.useStairs) {
      --currentState.levelIndex;
      currentState.useStairs = true;
    } else if (m_data.map.levels[levelIndex](nextPos).type == MapData::CellType::StairUp && !currentState.useStairs) {
      ++currentState.levelIndex;
      currentState.useStairs = true;
    } else {
      currentState.useStairs = false;
    }

//     gf::Log::debug("New target!\n");
    currentState.target = nextPos;
    m_moveCooldown = gf::seconds(0);
  }
}
