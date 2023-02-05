#ifndef RC_LIFT_SCENE_H
#define RC_LIFT_SCENE_H

#include <gf/Scene.h>
#include <gf/Action.h>
#include <gf/Animation.h>

#include "DialogEntity.h"

namespace rc {

  struct GameHub;

  enum class liftAnimation : int {
    Begin = 0,
    Middle = 1,
    End = 2,
  };

  class LiftScene : public gf::Scene {
  public:
    LiftScene(GameHub& game);

  private:
    void doHandleActions(gf::Window& window) override;
    void doUpdate(gf::Time time) override;
    void doRender(gf::RenderTarget& target, const gf::RenderStates& states) override;

  private:
    GameHub& m_game;

    gf::Action m_startAction;

    DialogEntity m_dialogEntity;
    gf::Animation m_beginLiftAnimation;
    gf::Animation m_middleLiftAnimation;
    gf::Animation m_endLiftAnimation;
    gf::Time m_liftCooldown;
    gf::Time m_liftAnimationCooldown;
    const gf::Time *m_currentCooldownLimit;
    const gf::Time *m_currentAnimationCooldownLimit;
    liftAnimation m_currentLiftAnimation;
    gf::Animation *m_currentAnimation;
  };

}

#endif // RC_LIFT_SCENE_H
