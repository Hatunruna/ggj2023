#include "MainScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace xy {

  constexpr float ViewSize = 500.0f;

  MainScene::MainScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_ltWorldView({ 0.0f, 0.0f }, { ViewSize, ViewSize })
  , m_rtWorldView({ 0.0f, 0.0f }, { ViewSize, ViewSize })
  , m_ltMap(game.data, game.state, Hero::Lisa)
  , m_rtMap(game.data, game.state, Hero::Ryan)
  , m_ltHero(game.data, game.state, Hero::Lisa)
  , m_rtHero(game.data, game.state, Hero::Ryan)
  , m_fullscreenAction("Fullscreen")
  , m_lisaUp("LisaUp")
  , m_lisaDown("LisaDown")
  , m_lisaLeft("LisaLeft")
  , m_lisaRight("LisaRight")
  , m_ryanUp("RyanUp")
  , m_ryanDown("RyanDown")
  , m_ryanLeft("RyanLeft")
  , m_ryanRight("RyanRight")
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

//     m_fullscreenAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Guide);
    addAction(m_fullscreenAction);

    m_lisaUp.addScancodeKeyControl(gf::Scancode::W);
    m_lisaUp.addScancodeKeyControl(gf::Scancode::Up);
    addAction(m_lisaUp);

    m_lisaDown.addScancodeKeyControl(gf::Scancode::S);
    m_lisaDown.addScancodeKeyControl(gf::Scancode::Down);
    addAction(m_lisaDown);

    m_lisaLeft.addScancodeKeyControl(gf::Scancode::A);
    m_lisaLeft.addScancodeKeyControl(gf::Scancode::Left);
    addAction(m_lisaLeft);

    m_lisaRight.addScancodeKeyControl(gf::Scancode::D);
    m_lisaRight.addScancodeKeyControl(gf::Scancode::Right);
    addAction(m_lisaRight);

    m_ryanUp.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Negative);
    m_ryanUp.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadUp);
    addAction(m_ryanUp);

    m_ryanDown.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftY, gf::GamepadAxisDirection::Positive);
    m_ryanDown.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadDown);
    addAction(m_ryanDown);

    m_ryanLeft.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Negative);
    m_ryanLeft.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadLeft);
    addAction(m_ryanLeft);

    m_ryanRight.addGamepadAxisControl(gf::AnyGamepad, gf::GamepadAxis::LeftX, gf::GamepadAxisDirection::Positive);
    m_ryanRight.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::DPadRight);
    addAction(m_ryanRight);
  }

  void MainScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    gf::Vector2i nextPosition = m_game.state.lisa.position;
    if (m_lisaUp.isActive()) {
      --nextPosition.y;
    } else if (m_lisaDown.isActive()){
      ++nextPosition.y;
    }
    if (m_lisaLeft.isActive()) {
      --nextPosition.x;
    } else if (m_lisaRight.isActive()) {
      ++nextPosition.x;
    }

    if (m_game.state.lisaMap.fieldOfView.isWalkable(nextPosition)) {
      m_game.state.lisa.position = nextPosition;
    }

    nextPosition = m_game.state.ryan.position;
    if (m_ryanUp.isActive()) {
      --nextPosition.y;
    } else if (m_ryanDown.isActive()){
      ++nextPosition.y;
    }
    if (m_ryanLeft.isActive()) {
      --nextPosition.x;
    } else if (m_ryanRight.isActive()) {
      ++nextPosition.x;
    }

    if (m_game.state.ryanMap.fieldOfView.isWalkable(nextPosition)) {
      m_game.state.ryan.position = nextPosition;
    }
  }

  void MainScene::doUpdate(gf::Time time) {
    m_ltWorldEntities.update(time);
    m_rtWorldEntities.update(time);
    m_ltHudEntities.update(time);
    m_rtHudEntities.update(time);

    m_ltWorldView.setCenter(m_game.state.lisa.position * CellSize + CellSize / 2);
    m_rtWorldView.setCenter(m_game.state.ryan.position * CellSize + CellSize / 2);

    // Update fov
    m_game.state.lisaMap.fieldOfView.clearFieldOfVision();
    m_game.state.lisaMap.fieldOfView.computeFieldOfVision(m_game.state.lisa.position, 2);
    m_game.state.ryanMap.fieldOfView.clearFieldOfVision();
    m_game.state.ryanMap.fieldOfView.computeFieldOfVision(m_game.state.ryan.position, 2);
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
