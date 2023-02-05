#ifndef RC_LIFT_SCENE_H
#define RC_LIFT_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Animation.h>

#include "DialogEntity.h"
#include "LiftEntity.h"

namespace rc {

  struct GameHub;

  class LiftScene : public gf::Scene {
  public:
    LiftScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;

  private:
    GameHub& m_game;

    gf::Action m_startAction;

    DialogEntity m_dialogEntity;
    LiftEntity m_liftEntity;
  };

}

#endif // RC_LIFT_SCENE_H
