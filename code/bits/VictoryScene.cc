#include "VictoryScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace rc {


  VictoryScene::VictoryScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_restartAction("Restart")
  , m_victoryEntity(game.resources)
  {
    setClearColor(gf::Color::Black);

    m_restartAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    m_restartAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
    m_restartAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
    m_restartAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);
    m_restartAction.addScancodeKeyControl(gf::Scancode::Space);
    addAction(m_restartAction);

    addHudEntity(m_victoryEntity);
  }

  void VictoryScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_restartAction.isActive()) {
      m_game.replaceScene(m_game.start, m_game.blackout, gf::seconds(1.0f));
    }
  }

}
