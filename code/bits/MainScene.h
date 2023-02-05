#ifndef RC_MAIN_SCENE_H
#define RC_MAIN_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Direction.h>

#include <SFML/Audio.hpp>

#include "HeroEntity.h"
#include "LightEntity.h"
#include "MapEntity.h"
#include "RootEntity.h"
#include "RootModel.h"
#include "SplitEntity.h"

namespace rc {

  struct GameHub;

struct HeroActions {
  HeroActions();

  static int s_index;

  gf::Action up;
  gf::Action down;
  gf::Action left;
  gf::Action right;
  gf::Action interact;
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

  RootModel m_rootModel;
  RootEntity m_ltRoot;
  RootEntity m_rtRoot;

  LightEntity m_light;
  SplitEntity m_split;

  // actions

  std::array<HeroActions, 2> heroActions;
  HeroActions& lisaActions = heroActions[0];
  HeroActions& ryanActions = heroActions[1];

  sf::Music& m_ambianceMusic;
};

}

#endif // RC_MAIN_SCENE_H
