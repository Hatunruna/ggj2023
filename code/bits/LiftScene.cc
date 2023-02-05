#include "LiftScene.h"

#include <gf/Color.h>

#include "GameHub.h"
#include "Settings.h"

namespace rc {

  namespace {
    constexpr gf::Time BeginLiftFrameDuration = gf::seconds(1.f / 25.f);
    constexpr gf::Time MiddleLiftFrameDuration = gf::seconds(1.f / 25.f);
    constexpr gf::Time EndLiftFrameDuration = gf::seconds(1.f / 25.f);
    constexpr gf::Time BeginLiftAnimationDuration = gf::seconds(2.f);
    constexpr gf::Time MiddleLiftAnimationDuration = gf::seconds(6.f);
    constexpr gf::Time EndLiftAnimationDuration = gf::seconds(2.f);
  }

  LiftScene::LiftScene(GameHub& game)
  : gf::Scene(game.getRenderer().getSize())
  , m_game(game)
  , m_startAction("Start")
  , m_dialogEntity(game.resources)
  , m_beginLiftAnimation()
  , m_middleLiftAnimation()
  , m_endLiftAnimation()
  , m_liftCooldown(gf::seconds(0))
  , m_liftAnimationCooldown(gf::seconds(0))
  , m_currentAnimation(&m_beginLiftAnimation)
  , m_currentLiftAnimation(liftAnimation::Begin)
  , m_currentCooldownLimit(&BeginLiftFrameDuration)
  , m_currentAnimationCooldownLimit(&BeginLiftAnimationDuration)
  {
    setClearColor(gf::Color::Black);

    for (int i = 0; i<14; ++i) {
      m_beginLiftAnimation.addFrame(m_game.resources.getTexture("images/lift/begin/begin_" + std::to_string(i) + ".png"), gf::RectF::fromSize(gf::vec(1.f, 1.f)), BeginLiftFrameDuration);
    }
    for (int i = 0; i<25; ++i) {
      m_middleLiftAnimation.addFrame(m_game.resources.getTexture("images/lift/middle/middle_" + std::to_string(i) + ".png"), gf::RectF::fromSize(gf::vec(1.f, 1.f)), MiddleLiftFrameDuration);
    }
    for (int i = 0; i<13; ++i) {
      m_endLiftAnimation.addFrame(m_game.resources.getTexture("images/lift/end/end_" + std::to_string(i) + ".png"), gf::RectF::fromSize(gf::vec(1.f, 1.f)), EndLiftFrameDuration);
    }

    m_beginLiftAnimation.setLoop(false);
    m_endLiftAnimation.setLoop(false);

    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::A);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::B);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::X);
    m_startAction.addGamepadButtonControl(gf::AnyGamepad, gf::GamepadButton::Y);
    m_startAction.addScancodeKeyControl(gf::Scancode::Space);
    addAction(m_startAction);

    addHudEntity(m_dialogEntity);
  }

  void LiftScene::doUpdate([[maybe_unused]] gf::Time time) {
    m_liftCooldown += time;
    m_liftAnimationCooldown += time;

    if (m_liftCooldown > *m_currentCooldownLimit) {
      m_liftCooldown -= *m_currentCooldownLimit;
      m_currentAnimation->update(time);
    }

    if (m_liftAnimationCooldown > *m_currentAnimationCooldownLimit) {
      m_liftAnimationCooldown -= *m_currentAnimationCooldownLimit;
      m_liftCooldown = gf::seconds(0);
      switch (m_currentLiftAnimation) {
        case liftAnimation::Begin:
          fprintf(stderr, "begin to middle\n");
          m_currentLiftAnimation = liftAnimation::Middle;
          m_currentAnimation = &m_middleLiftAnimation;
        break;
        case liftAnimation::Middle:
          fprintf(stderr, "middle to end\n");
          m_currentLiftAnimation = liftAnimation::End;
          m_currentAnimation = &m_endLiftAnimation;
        break;
        case liftAnimation::End:
          fprintf(stderr, "eend\n");
          m_game.replaceAllScenes(m_game.main);
        break;
        default:
          assert(false);
      }
    }
  }

  void LiftScene::doRender(gf::RenderTarget& target, const gf::RenderStates& states) {
    gf::AnimatedSprite animatedSprite;
    animatedSprite.setAnimation(*m_currentAnimation);
    animatedSprite.setPosition({0.f,0.f});
    animatedSprite.setAnchor(gf::Anchor::TopLeft);
    target.draw(animatedSprite, states);
    renderHudEntities(target, states);
  }

  void LiftScene::doHandleActions([[maybe_unused]] gf::Window& window) {
    if (!isActive()) {
      return;
    }

    if (m_startAction.isActive()) {
      m_game.replaceAllScenes(m_game.main);
    }
  }

}
