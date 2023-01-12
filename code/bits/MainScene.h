#ifndef XY_MAIN_SCENE_H
#define XY_MAIN_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

namespace xy {

  struct GameHub;

  class MainScene : public gf::Scene {
  public:
    MainScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    GameHub& m_game;

    // views

    gf::ExtendView m_ltWorldView;
    gf::ScreenView m_ltHudView;

    gf::ExtendView m_rtWorldView;
    gf::ScreenView m_rtHudView;

    // entities

    gf::EntityContainer m_ltWorldEntities;
    gf::EntityContainer m_ltHudEntities;

    gf::EntityContainer m_rtWorldEntities;
    gf::EntityContainer m_rtHudEntities;

    // actions

    gf::Action m_fullscreenAction;
  };

}

#endif // XY_MAIN_SCENE_H
