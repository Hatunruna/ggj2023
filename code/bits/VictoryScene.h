#ifndef RC_VICTORY_SCENE_H
#define RC_VICTORY_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

#include "VictoryEntity.h"

namespace rc {

  struct GameHub;

  class VictoryScene : public gf::Scene {
  public:
    VictoryScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;

  private:
    GameHub& m_game;

    gf::Action m_restartAction;

    VictoryEntity m_victoryEntity;
  };

}

#endif // RC_GAME_OVER_SCENE_H
