#include "StartScene.h"

#include <gf/Color.h>

#include "GameHub.h"

namespace xy {

  StartScene::StartScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_startAction("Start")
  , m_fullscreenAction("Fullscreen")
  , m_titleEntity(game.resources)
  {
    setClearColor(gf::Color::Black);

    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);
    m_startAction.addScancodeKeyControl(gf::Scancode::Space);
    addAction(m_startAction);

    m_fullscreenAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Back);
    m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    addAction(m_fullscreenAction);

    addHudEntity(m_titleEntity);
  }

  void StartScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (m_startAction.isActive()) {
      auto levels = createProceduralMap(m_game.random);

      m_game.state.lisaMap.levels.clear();
      m_game.state.ryanMap.levels.clear();

      for (auto & level : levels) {
        m_game.state.lisaMap.levels.push_back(MapLevel(level));
        m_game.state.ryanMap.levels.push_back(MapLevel(level));
      }

      // TODO: set position from generated map
      m_game.state.lisa.position = gf::vec(1, 1);
      m_game.state.ryan.position = gf::vec(3, 1);

      m_game.replaceScene(m_game.main); //, m_game.blackout, gf::seconds(TransitionDelay));
    }
  }

}
