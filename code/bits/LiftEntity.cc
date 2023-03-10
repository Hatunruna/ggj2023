#include "LiftEntity.h"

#include <gf/AnimatedSprite.h>
#include <gf/RenderTarget.h>

namespace rc {

  namespace {
    constexpr gf::Time WarmUpDuration = gf::seconds(1.0f);

    constexpr int BeginFrames = 15;
    constexpr int MiddleFrames = 26;
    constexpr int EndFrames = 14;

    constexpr gf::Time Framerate = gf::seconds(1.0f / 25.0f);

  }

  LiftEntity::LiftEntity(gf::ResourceManager& resources)
  : m_currentAnimation(nullptr)
  , m_state(LiftAnimationState::WarmUp)
  , m_elapsedTime(gf::Time::zero())
  {
    // Load begin animation
    for (int i = 0; i < BeginFrames; ++i) {
      gf::Texture& texture = resources.getTexture("images/lift/begin/begin_" + std::to_string(i) + ".png");
      texture.setSmooth();

      m_liftStartAnimation.addFrame(texture, gf::RectF::fromSize(gf::vec(1.0f, 1.0f)), (i == 0) ? gf::seconds(2.0f) : Framerate);
    }
    m_liftStartAnimation.setLoop(false);

    // Load middle animation
    for (int i = 0; i < MiddleFrames; ++i) {
      gf::Texture& texture = resources.getTexture("images/lift/middle/middle_" + std::to_string(i) + ".png");
      texture.setSmooth();

      m_liftMiddleAnimation.addFrame(texture, gf::RectF::fromSize(gf::vec(1.0f, 1.0f)), Framerate);
    }
    m_liftMiddleAnimation.setLoop(true);

    // Load end animation
    for (int i = 0; i < EndFrames; ++i) {
      gf::Texture& texture = resources.getTexture("images/lift/end/end_" + std::to_string(i) + ".png");
      texture.setSmooth();

      m_liftEndAnimation.addFrame(texture, gf::RectF::fromSize(gf::vec(1.0f, 1.0f)), Framerate);
    }
    m_liftEndAnimation.setLoop(false);

    m_currentAnimation = &m_liftStartAnimation;
  }

  void LiftEntity::update(gf::Time time) {
    const gf::Time MiddleDuration = gf::seconds(75.0f);

    m_elapsedTime += time;

    switch (m_state) {
      case LiftAnimationState::WarmUp: {
        if (m_elapsedTime >= WarmUpDuration) {
          m_elapsedTime -= WarmUpDuration;
          m_liftStartAnimation.reset();
          m_currentAnimation = &m_liftStartAnimation;
          m_state = LiftAnimationState::Begin;
        } else {
          return;
        }
      }

      case LiftAnimationState::Begin:
        if (m_currentAnimation->isFinished()) {
          m_elapsedTime = gf::Time::zero();
          m_liftMiddleAnimation.reset();
          m_currentAnimation = &m_liftMiddleAnimation;
          m_state = LiftAnimationState::Middle;
        }
        break;

      case LiftAnimationState::Middle:
        if (m_elapsedTime >= MiddleDuration) {
          m_elapsedTime -= MiddleDuration;
          m_liftEndAnimation.reset();
          m_currentAnimation = &m_liftEndAnimation;
          m_state = LiftAnimationState::End;
        }
        break;

      case LiftAnimationState::End:
        // TODO
        break;
    }

    m_currentAnimation->update(time);
  }

  void LiftEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    if (m_state == LiftAnimationState::WarmUp) {
      return;
    }

    gf::Coordinates coords(target);
    gf::AnimatedSprite sprite;
    sprite.setAnimation(*m_currentAnimation);
    sprite.setPosition(gf::vec(0.0f, 0.0f));
    float scale = coords.getWindowSize().y / 1080.f;
    sprite.setScale(scale);
    target.draw(sprite, states);
  }

}
