#include "LiftScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace rc {

  LiftScene::LiftScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_startAction("Start")
  , m_dialogEntity(game.resources)
  , m_liftEntity(game.resources)
  {
    setClearColor(gf::Color::Black);

    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);
    m_startAction.addScancodeKeyControl(gf::Scancode::Space);
    addAction(m_startAction);

    addHudEntity(m_liftEntity);
    addHudEntity(m_dialogEntity);
  }


  void LiftScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_startAction.isActive()) {
      m_game.replaceScene(m_game.main, m_game.blackout, gf::seconds(1.0f));
    }
  }

}
