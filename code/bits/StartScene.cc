#include "StartScene.h"

#include <gf/Color.h>

#include "GameHub.h"

namespace xy {

  StartScene::StartScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_startAction("Start")
  , m_titleEntity(game.resources)
  {
    setClearColor(gf::Color::Black);

    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);
    m_startAction.addScancodeKeyControl(gf::Scancode::Space);
    addAction(m_startAction);

    addHudEntity(m_titleEntity);
  }

  void StartScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_startAction.isActive()) {
      auto levels = createProceduralMap(m_game.random);

      m_game.state.lisa.map.levels.clear();
      m_game.state.ryan.map.levels.clear();

      bool first = true;

      for (auto & level : levels) {
        if (first) {
          first = false;

          auto [ lisa, ryan ] = computeStartingPositions(level);
          m_game.state.lisa.hero.position = m_game.state.lisa.hero.target = lisa;
          m_game.state.ryan.hero.position = m_game.state.ryan.hero.target = ryan;

          auto mapLevel = MapLevel(level, m_game.random);
          m_game.state.lisa.map.levels.push_back(mapLevel);
          m_game.state.ryan.map.levels.push_back(std::move(mapLevel));
        } else {
          auto mapLevel = MapLevel(level, m_game.random);
          m_game.state.lisa.map.levels.push_back(mapLevel);
          m_game.state.ryan.map.levels.push_back(std::move(mapLevel));
        }
      }

      // TODO: set position from generated map

      m_game.replaceScene(m_game.main); //, m_game.blackout, gf::seconds(TransitionDelay));
    }
  }

}
