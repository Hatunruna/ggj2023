#include "CommonScene.h"

#include <gf/Color.h>

#include "GameHub.h"

namespace rc {

  CommonScene::CommonScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_quitAction("Quit")
  , m_fullscreenAction("Fullscreen")
  {
    setClearColor(gf::Color::Black);

    m_quitAction.addScancodeKeyControl(gf::Scancode::Escape);
    addAction(m_quitAction);

    m_fullscreenAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Back);
    m_fullscreenAction.addKeycodeKeyControl(gf::Keycode::F);
    addAction(m_fullscreenAction);
  }

  void CommonScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (m_fullscreenAction.isActive()) {
      window.toggleFullscreen();
    }

    if (m_quitAction.isActive()) {
      window.close();
    }
  }

}
