#ifndef XY_ROOT_ENTITY_H
#define XY_ROOT_ENTITY_H

#include <gf/Entity.h>
#include <gf/Random.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>

#include "Hero.h"

namespace xy {
  struct GameState;

  class RootEntity : public gf::Entity {
  public:
    RootEntity(GameState& state, Hero hero, gf::ResourceManager& resources, gf::Random& random);
    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
  private:
    GameState& m_state;
    Hero m_hero;
    gf::Texture& m_texture;
    gf::Random& m_random;
    gf::Time m_time;
  };

}

#endif // XY_ROOT_ENTITY_H
