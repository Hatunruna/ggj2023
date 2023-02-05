#include "MainScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace rc {

  constexpr float ViewSize = 1000.0f;
  // constexpr float ViewSize = 5000.0f;
  constexpr int ViewRadius = 7;

  HeroActions::HeroActions()
  : up("Up")
  , down("Down")
  , left("Left")
  , right("Right")
  , interact("Interact")
  {
  }

  MainScene::MainScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_ltWorldView({ 0.0f, 0.0f }, { ViewSize, ViewSize })
  , m_rtWorldView({ 0.0f, 0.0f }, { ViewSize, ViewSize })
  , m_ltMap(game, game.state, Hero::Lisa)
  , m_rtMap(game, game.state, Hero::Ryan)
  , m_ltHero(game.state, Hero::Lisa, m_game.resources)
  , m_rtHero(game.state, Hero::Ryan, m_game.resources)
  , m_rootModel(game.state, m_game.random)
  , m_ltRoot(game.state, Hero::Lisa, m_game.resources)
  , m_rtRoot(game.state, Hero::Ryan, m_game.resources)
  , m_light(game.state)
  {
    auto ltViewport = gf::RectF::fromPositionSize({ 0.0f, 0.0f }, { 0.5f, 1.0f });
    m_ltWorldView.setViewport(ltViewport);
    m_ltHudView.setViewport(ltViewport);
    auto rtViewport = gf::RectF::fromPositionSize({ 0.5f, 0.0f }, { 0.5f, 1.0f });
    m_rtWorldView.setViewport(rtViewport);
    m_rtHudView.setViewport(rtViewport);

    addView(m_ltWorldView);
    addView(m_ltHudView);
    addView(m_rtWorldView);
    addView(m_rtHudView);

    setClearColor(gf::Color::Black);

    addModel(m_rootModel);

    m_ltWorldEntities.addEntity(m_ltMap);
    m_ltWorldEntities.addEntity(m_ltHero);
    m_ltWorldEntities.addEntity(m_ltRoot);

    m_rtWorldEntities.addEntity(m_rtMap);
    m_rtWorldEntities.addEntity(m_rtHero);
    m_rtWorldEntities.addEntity(m_rtRoot);

    m_ltHero.m_otherEntity = &m_rtHero;
    m_rtHero.m_otherEntity = &m_ltHero;

    addHudEntity(m_light);
    addHudEntity(m_split);

    lisaActions.up.addScancodeKeyControl(gf::Scancode::W);
    lisaActions.up.setContinuous();
    addAction(lisaActions.up);

    lisaActions.down.addScancodeKeyControl(gf::Scancode::S);
    lisaActions.down.setContinuous();
    addAction(lisaActions.down);

    lisaActions.left.addScancodeKeyControl(gf::Scancode::A);
    lisaActions.left.setContinuous();
    addAction(lisaActions.left);

    lisaActions.right.addScancodeKeyControl(gf::Scancode::D);
    lisaActions.right.setContinuous();
    addAction(lisaActions.right);

    lisaActions.interact.addKeycodeKeyControl(gf::Keycode::Space);
    addAction(lisaActions.interact);

    ryanActions.up.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
    ryanActions.up.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadUp);
    ryanActions.up.addScancodeKeyControl(gf::Scancode::Up);
    ryanActions.up.setContinuous();
    addAction(ryanActions.up);

    ryanActions.down.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
    ryanActions.down.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadDown);
    ryanActions.down.addScancodeKeyControl(gf::Scancode::Down);
    ryanActions.down.setContinuous();
    addAction(ryanActions.down);

    ryanActions.left.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Negative);
    ryanActions.left.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadLeft);
    ryanActions.left.addScancodeKeyControl(gf::Scancode::Left);
    ryanActions.left.setContinuous();
    addAction(ryanActions.left);

    ryanActions.right.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Positive);
    ryanActions.right.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadRight);
    ryanActions.right.addScancodeKeyControl(gf::Scancode::Right);
    ryanActions.right.setContinuous();
    addAction(ryanActions.right);

    ryanActions.interact.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    ryanActions.interact.addKeycodeKeyControl(gf::Keycode::Return);
    addAction(ryanActions.interact);
  }

  void MainScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    auto updateHeroDir = [this](Hero hero) {
      HeroEntity& entity = (hero == Hero::Lisa) ? m_ltHero : m_rtHero;
      HeroActions& actions = heroActions[static_cast<int>(hero)];
      bool finished = false;

      if (actions.right.isActive()) {
        finished = entity.move(gf::Direction::Right);
      } else if (actions.left.isActive()) {
        finished = entity.move(gf::Direction::Left);
      } else if (actions.down.isActive()) {
        finished = entity.move(gf::Direction::Down);
      } else if (actions.up.isActive()) {
        finished = entity.move(gf::Direction::Up);
      } else {
        entity.move(gf::Direction::Center);
      }

      if (finished) {
        m_game.state.status = GameStatus::Victory;
      }
    };

    updateHeroDir(Hero::Lisa);
    updateHeroDir(Hero::Ryan);

    // Handle interact
    auto handleInteract = [this](Hero hero) {
      const int heroIndex = static_cast<int>(hero);

      HeroActions& actions = heroActions[heroIndex];
      HeroState& heroState = m_game.state.players[heroIndex].hero;
      const int levelIndex = heroState.levelIndex;
      MapLevel& level = m_game.state.players[heroIndex].map.levels[levelIndex];

      auto checkComputer = [this, &level, levelIndex](gf::Vector2i position) -> bool {
        const MapCell& cell = level.level.cells(position);

        if (cell.type == MapCellType::Computer) {
          for (auto& playerState: m_game.state.players) {
            gf::Vector2i doorPosition = cell.computerState.controlledDoor;
            MapCell& door = playerState.map.levels[levelIndex].level.cells(doorPosition);
            door.doorState.isOpen = true;
            playerState.map.levels[levelIndex].map.setEmpty(doorPosition);
          }

          return true;
        }

        return false;
      };

      if (actions.interact.isActive()) {
        if (checkComputer(heroState.position + gf::vec(0, 1))) {
        } else if (checkComputer(heroState.position + gf::vec(0, -1))) {
        } else if (checkComputer(heroState.position + gf::vec(1, 0))) {
        } else if (checkComputer(heroState.position + gf::vec(-1, 0))) {
        }
      }
    };

    handleInteract(Hero::Lisa);
    handleInteract(Hero::Ryan);
  }

  void MainScene::doUpdate(gf::Time time) {
    if (m_game.state.status == GameStatus::Victory) {
      m_game.replaceScene(m_game.victory);
    }

    if (m_game.state.status == GameStatus::GameOver) {
      m_game.replaceScene(m_game.gameOver);
    }

    m_ltWorldEntities.update(time);
    m_rtWorldEntities.update(time);
    m_ltHudEntities.update(time);
    m_rtHudEntities.update(time);

    m_ltWorldView.setCenter(gf::Vector2f(m_game.state.lisa.hero.middle) + gf::Vector2f(CellSize) * 0.5f);
    m_rtWorldView.setCenter(gf::Vector2f(m_game.state.ryan.hero.middle) + gf::Vector2f(CellSize) * 0.5f);

    auto updateFov = [this](Hero hero) {
      const int index = static_cast<int>(hero);
      const HeroState& heroState = m_game.state.localPlayer(hero).hero;
      gf::SquareMap& map = m_game.state.localPlayer(hero).map.levels[heroState.levelIndex].map;

      map.clearFieldOfVision();
      map.computeFieldOfVision(heroState.position, ViewRadius);
    };

    updateFov(Hero::Lisa);
    updateFov(Hero::Ryan);
  }

  void MainScene::doRender(gf::RenderTarget& target, const gf::RenderStates& states) {
    target.setView(m_ltWorldView);
    m_ltWorldEntities.render(target, states);
    target.setView(m_rtWorldView);
    m_rtWorldEntities.render(target, states);

    target.setView(m_ltHudView);
    m_ltHudEntities.render(target, states);
    target.setView(m_rtHudView);
    m_rtHudEntities.render(target, states);

    renderHudEntities(target, states);
  }

}
