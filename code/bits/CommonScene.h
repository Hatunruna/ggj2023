#ifndef XY_COMMON_SCENE_H
#define XY_COMMON_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>

namespace xy {

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

#endif // XY_COMMON_SCENE_H
