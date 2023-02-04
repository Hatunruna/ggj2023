#ifndef XY_MAIN_SCENE_H
#define XY_MAIN_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Direction.h>

#include "HeroEntity.h"
#include "MapEntity.h"
#include "SplitEntity.h"

namespace xy {

  struct GameHub;

struct HeroActions {
  HeroActions();

  static int s_index;

  gf::Action up;
  gf::Action down;
  gf::Action left;
  gf::Action right;
};

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

  std::array<HeroActions, 2> heroActions;
  HeroActions& lisaActions = heroActions[0];
  HeroActions& ryanActions = heroActions[1];
};

}

#endif // XY_MAIN_SCENE_H
