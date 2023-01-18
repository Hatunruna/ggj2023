#ifndef XY_MAIN_SCENE_H
#define XY_MAIN_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

#include "HeroEntity.h"
#include "MapEntity.h"
#include "SplitEntity.h"

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

    // containers

    gf::EntityContainer m_ltWorldEntities;
    gf::EntityContainer m_ltHudEntities;

    gf::EntityContainer m_rtWorldEntities;
    gf::EntityContainer m_rtHudEntities;

    // entities

    MapEntity m_ltMap;
    MapEntity m_rtMap;

    HeroEntity m_ltHero;
    HeroEntity m_rtHero;


    SplitEntity m_split;

    // actions

    gf::Action m_fullscreenAction;

    gf::Action m_lisaUp;
    gf::Action m_lisaDown;
    gf::Action m_lisaLeft;
    gf::Action m_lisaRight;

    gf::Action m_ryanUp;
    gf::Action m_ryanDown;
    gf::Action m_ryanLeft;
    gf::Action m_ryanRight;
  };

}

#endif // XY_MAIN_SCENE_H
