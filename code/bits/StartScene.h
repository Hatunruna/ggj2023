#ifndef RC_START_SCENE_H
#define RC_START_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

#include "TitleEntity.h"

namespace rc {

  struct GameHub;

  class StartScene : public gf::Scene {
  public:
    StartScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;

  private:
    GameHub& m_game;

    gf::Action m_startAction;

    TitleEntity m_titleEntity;
  };

}

#endif // RC_START_SCENE_H
