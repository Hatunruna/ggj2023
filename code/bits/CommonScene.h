#ifndef RC_COMMON_SCENE_H
#define RC_COMMON_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

namespace rc {

  struct GameHub;

  class CommonScene : public gf::Scene {
  public:
    CommonScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;

  private:
    GameHub& m_game;
    gf::Action m_quitAction;
    gf::Action m_fullscreenAction;
  };

}

#endif // RC_COMMON_SCENE_H
