#ifndef RC_LIFT_ENTITY_H
#define RC_LIFT_ENTITY_H

#include <gf/Animation.h>
#include <gf/Coordinates.h>
#include <gf/Entity.h>
#include <gf/ResourceManager.h>

namespace rc {

  enum class LiftAnimationState {
    WarmUp,
    Begin,
    Middle,
    End,
  };

  class LiftEntity : public gf::Entity {
    public:
      LiftEntity(gf::ResourceManager& resources);

      void update(gf::Time time) override;
      void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

    private:
      gf::Animation m_liftStartAnimation;
      gf::Animation m_liftMiddleAnimation;
      gf::Animation m_liftEndAnimation;

      gf::Animation* m_currentAnimation;

      LiftAnimationState m_state;
      gf::Time m_elapsedTime;
  };

}

#endif // RC_LIFT_ENTITY_H
