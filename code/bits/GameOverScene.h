#ifndef RC_GAME_OVER_SCENE_H
#define RC_GAME_OVER_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

#include "GameOverEntity.h"

namespace rc {

  struct GameHub;

  class GameOverScene : public gf::Scene {
  public:
    GameOverScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;

  private:
    GameHub& m_game;

    gf::Action m_restartAction;

    GameOverEntity m_gameOverEntity;
  };

}

#endif // RC_GAME_OVER_SCENE_H
