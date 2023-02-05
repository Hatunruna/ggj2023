#include "HeroEntity.h"

#include <gf/Shapes.h>
#include <gf/RenderTarget.h>
#include <gf/ResourceManager.h>

#include "GameState.h"
#include "Settings.h"

namespace rc {

  namespace {

    constexpr gf::Time MoveCooldown = gf::seconds(0.25f);
    constexpr float TextureScale = 64.0f / 256.0f;

    gf::Texture& getHeroTexture(gf::ResourceManager& resources, const std::string& textureName, Hero hero) {
      std::string dir;
      switch(hero) {
      case Hero::Lisa:
        dir = "lisa/";
        break;

      case Hero::Ryan:
        dir = "ryan/";
        break;
      }

      return resources.getTexture("images/" + dir + textureName);
    }

  }

  HeroEntity::HeroEntity(GameState& state, Hero hero, gf::ResourceManager& resources)
  : m_state(state)
  , m_hero(hero)
  , m_resources(resources)
  , m_stillRunning(false)
  , m_pauseTopTexture(getHeroTexture(m_resources, "pause_top.png", hero))
  , m_pauseLeftRightTexture(getHeroTexture(m_resources, "pause_left_right.png", hero))
  , m_pauseBottomTexture(getHeroTexture(m_resources, "pause_bottom.png", hero))
  , m_runTopTexture(getHeroTexture(m_resources, "run_top.png", hero))
  , m_runLeftRightTexture(getHeroTexture(m_resources, "run_left_right.png", hero))
  , m_runBottomTexture(getHeroTexture(m_resources, "run_bottom.png", hero))
  , m_currentAnimation(nullptr)
  , m_currentDirection(gf::Direction::Down)
  {
    m_pauseTopTexture.setSmooth();
    m_pauseLeftRightTexture.setSmooth();
    m_pauseBottomTexture.setSmooth();
    m_runTopTexture.setSmooth();
    m_runLeftRightTexture.setSmooth();
    m_runBottomTexture.setSmooth();

    m_runTopAnimation.addTileset(m_runTopTexture, gf::vec(4, 2), gf::seconds(1.0f / 25.0f), 8);
    m_runLeftRightAnimation.addTileset(m_runLeftRightTexture, gf::vec(4, 2), gf::seconds(1.0f / 25.0f), 8);
    m_runBottomAnimation.addTileset(m_runBottomTexture, gf::vec(4, 2), gf::seconds(1.0f / 25.0f), 8);
  }

  void HeroEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    const HeroState& localState = m_state.localPlayer(m_hero).hero;
    const HeroState& otherState = m_state.otherPlayer(m_hero).hero;
    const gf::SquareMap& localLevel = m_state.localPlayer(m_hero).map.levels[localState.levelIndex].map;

    renderAnimation(target, states);
    if (localState.levelIndex == otherState.levelIndex && localLevel.isInFieldOfVision(otherState.position)) {
      m_otherEntity->renderAnimation(target, states);
    }
  }

  void HeroEntity::renderAnimation(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Sprite sprite;
    const HeroState& localState = m_state.localPlayer(m_hero).hero;

    switch (m_currentDirection) {
      case gf::Direction::Up:
        sprite = m_pauseTopTexture;
        m_currentAnimation = &m_runTopAnimation;
        break;
      case gf::Direction::Down:
        sprite = m_pauseBottomTexture;
        m_currentAnimation = &m_runBottomAnimation;
        break;
      case gf::Direction::Right:
        sprite = m_pauseLeftRightTexture;
        m_currentAnimation = &m_runLeftRightAnimation;
        break;
      case gf::Direction::Left:
        sprite = m_pauseLeftRightTexture;
        m_currentAnimation = &m_runLeftRightAnimation;
        break;
      default:
        assert(false);
    }

    if (m_currentAnimation == nullptr || (localState.position == localState.target && !m_stillRunning)) {
      sprite.setPosition(localState.middle + CellSize / 2);
      sprite.setAnchor(gf::Anchor::Center);
      sprite.scale(TextureScale);
      if (gf::Direction::Right == m_currentDirection) {
        sprite.scale({-1.f, 1.f});
      }
      target.draw(sprite, states);
    } else {
      gf::AnimatedSprite animatedSprite;
      animatedSprite.setAnimation(*m_currentAnimation);
      animatedSprite.setPosition(localState.middle + CellSize / 2);
      animatedSprite.setAnchor(gf::Anchor::Center);
      animatedSprite.scale(TextureScale);
      if (gf::Direction::Right == m_currentDirection) {
        animatedSprite.scale({-1.f, 1.f});
      }
      target.draw(animatedSprite, states);
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
      if (m_currentAnimation != nullptr) {
        m_currentAnimation->update(time);
      }
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
