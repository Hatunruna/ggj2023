#include "HeroEntity.h"

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>

#include "GameState.h"
#include "Settings.h"

namespace xy {

  namespace {

    constexpr gf::Time MoveCooldown = gf::seconds(0.25f);

  }

  HeroEntity::HeroEntity(GameState& state, Hero hero)
  : m_state(state)
  , m_hero(hero)
  , m_stillRunning(false)
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
      hero.setPosition(heroState.middle + CellSize / 2);
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

  void HeroEntity::update(gf::Time time) {
    m_moveCooldown += time;
    HeroState& currentState = m_state.localPlayer(m_hero).hero;

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
    HeroState& currentState = m_state.localPlayer(m_hero).hero;
    HeroState& otherState = m_state.otherPlayer(m_hero).hero;

    if (dir == gf::Direction::Center) {
      m_stillRunning = false;
      return;
    }

    m_stillRunning = true;

    if (m_moveCooldown < MoveCooldown) {
//       gf::Log::debug("Cooldown not yet finished!\n");
      return;
    }

    m_currentDirection = dir;

    gf::Vector2i nextPos = currentState.position + gf::displacement(dir);

    const int levelIndex = currentState.levelIndex;
    const MapLevel& level = m_state.localPlayer(m_hero).map.levels[levelIndex];

    bool nextPositionAlreadyOccupied =
      (otherState.levelIndex == levelIndex)
      && (otherState.position == nextPos);
    if (nextPos == currentState.position || !level.map.isWalkable(nextPos) || nextPositionAlreadyOccupied) {
//       gf::Log::debug("Collision!\n");
      return;
    }

    // stairs things
    if (level.cells(nextPos).type == MapCellType::StairDown && !currentState.useStairs) {
      --currentState.levelIndex;
      currentState.useStairs = true;
    } else if (level.cells(nextPos).type == MapCellType::StairUp && !currentState.useStairs) {
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
