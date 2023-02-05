#ifndef RC_HERO_ENTITY_H
#define RC_HERO_ENTITY_H

#include <gf/Entity.h>
#include <gf/Direction.h>
#include <gf/Animation.h>
#include <gf/Sprite.h>
#include <gf/AnimatedSprite.h>
#include <gf/ResourceManager.h>

#include "Hero.h"

namespace rc {
  struct GameState;

  class HeroEntity : public gf::Entity {
  public:
    HeroEntity(GameState& state, Hero hero, gf::ResourceManager& resources);

    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
    bool move(gf::Direction dir);
    void renderAnimation(gf::RenderTarget &target, const gf::RenderStates &states);

    HeroEntity *m_otherEntity;

  private:
    GameState& m_state;
    Hero m_hero;
    gf::Time m_moveCooldown;
    gf::Direction m_currentDirection = gf::Direction::Center;
    bool m_stillRunning;
    gf::ResourceManager& m_resources;


    gf::Texture& m_pauseTopTexture;
    gf::Texture&  m_runTopTexture;
    gf::Animation m_runTopAnimation;

    gf::Texture& m_pauseLeftRightTexture;
    gf::Texture&  m_runLeftRightTexture;
    gf::Animation m_runLeftRightAnimation;

    gf::Texture& m_pauseBottomTexture;
    gf::Texture&  m_runBottomTexture;
    gf::Animation m_runBottomAnimation;

    gf::Animation* m_currentAnimation;
    gf::Animation* m_otherCurrentAnimation;
  };

}

#endif // RC_HERO_ENTITY_H
