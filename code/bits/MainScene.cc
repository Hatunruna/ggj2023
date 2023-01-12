#include "MainScene.h"

#include <gf/Color.h>

#include "GameHub.h"

namespace xy {

  constexpr float ViewSize = 500.0f;

  MainScene::MainScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_ltWorldView({ 0.0f, 0.0f }, { ViewSize, ViewSize })
  , m_rtWorldView({ 0.0f, 0.0f }, { ViewSize, ViewSize })
  , m_fullscreenAction("Fullscreen")
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

//     m_fullscreenAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Guide);
    addAction(m_fullscreenAction);
  }

  void MainScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

  }

  void MainScene::doUpdate(gf::Time time) {
    m_ltWorldEntities.update(time);
    m_rtWorldEntities.update(time);
    m_ltHudEntities.update(time);
    m_rtHudEntities.update(time);
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
