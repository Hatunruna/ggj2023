#include "MainScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace xy {

  constexpr float ViewSize = 1000.0f;

  HeroActions::HeroActions()
  : up("Up")
  , down("Down")
  , left("Left")
  , right("Right") {
  }

  MainScene::MainScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_ltWorldView({ 0.0f, 0.0f }, { ViewSize, ViewSize })
  , m_rtWorldView({ 0.0f, 0.0f }, { ViewSize, ViewSize })
  , m_ltMap(game.state, Hero::Lisa)
  , m_rtMap(game.state, Hero::Ryan)
  , m_ltHero(game.state, Hero::Lisa)
  , m_rtHero(game.state, Hero::Ryan)
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

    m_ltWorldEntities.addEntity(m_ltMap);
    m_ltWorldEntities.addEntity(m_ltHero);

    m_rtWorldEntities.addEntity(m_rtMap);
    m_rtWorldEntities.addEntity(m_rtHero);


    addHudEntity(m_split);

    lisaActions.up.addScancodeKeyControl(gf::Scancode::W);
    addAction(lisaActions.up);

    lisaActions.down.addScancodeKeyControl(gf::Scancode::S);
    addAction(lisaActions.down);

    lisaActions.left.addScancodeKeyControl(gf::Scancode::A);
    addAction(lisaActions.left);

    lisaActions.right.addScancodeKeyControl(gf::Scancode::D);
    addAction(lisaActions.right);

    ryanActions.up.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
    ryanActions.up.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadUp);
    ryanActions.up.addScancodeKeyControl(gf::Scancode::Up);
    addAction(ryanActions.up);

    ryanActions.down.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
    ryanActions.down.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadDown);
    ryanActions.down.addScancodeKeyControl(gf::Scancode::Down);
    addAction(ryanActions.down);

    ryanActions.left.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Negative);
    ryanActions.left.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadLeft);
    ryanActions.left.addScancodeKeyControl(gf::Scancode::Left);
    addAction(ryanActions.left);

    ryanActions.right.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Positive);
    ryanActions.right.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadRight);
    ryanActions.right.addScancodeKeyControl(gf::Scancode::Right);
    addAction(ryanActions.right);
  }

  void MainScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    auto updateHeroPosition = [this](Hero hero) {
      const int index = static_cast<int>(hero);

      HeroActions& actions = heroActions[index];
      HeroState& heroState = m_game.state.players[index].hero;
      const int levelIndex = heroState.levelIndex;
      const MapLevel& level = m_game.state.players[index].map.levels[levelIndex];

      const HeroState& otherHeroState = m_game.state.players[1 - index].hero;

      gf::Vector2i nextPosition = heroState.position;
      if (actions.up.isActive()) {
        --nextPosition.y;
      } else if (actions.down.isActive()){
        ++nextPosition.y;
      }
      if (actions.left.isActive()) {
        --nextPosition.x;
      } else if (actions.right.isActive()) {
        ++nextPosition.x;
      }

      bool nextPositionAlreadyOccupied =
        (otherHeroState.levelIndex == levelIndex)
        && (otherHeroState.position == nextPosition);
      if (nextPosition != heroState.position && level.map.isWalkable(nextPosition) && !nextPositionAlreadyOccupied) {
        heroState.position = nextPosition;
        heroState.useStairs = false;
      }

      if (level.cells(nextPosition).type == MapCellType::StairDown && !heroState.useStairs) {
        --heroState.levelIndex;
        heroState.useStairs = true;
      } else if (level.cells(nextPosition).type == MapCellType::StairUp && !heroState.useStairs) {
        ++heroState.levelIndex;
        heroState.useStairs = true;
      }
    };

    updateHeroPosition(Hero::Lisa);
    updateHeroPosition(Hero::Ryan);
  }

  void MainScene::doUpdate(gf::Time time) {
    m_ltWorldEntities.update(time);
    m_rtWorldEntities.update(time);
    m_ltHudEntities.update(time);
    m_rtHudEntities.update(time);

    m_ltWorldView.setCenter(m_game.state.lisa.hero.position * CellSize + CellSize / 2);
    m_rtWorldView.setCenter(m_game.state.ryan.hero.position * CellSize + CellSize / 2);

    auto updateFov = [this](Hero hero) {
      const int index = static_cast<int>(hero);
      const HeroState& heroState = m_game.state.localPlayer(hero).hero;
      gf::SquareMap& map = m_game.state.localPlayer(hero).map.levels[heroState.levelIndex].map;

      map.clearFieldOfVision();
      map.computeFieldOfVision(heroState.position, 3);
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
