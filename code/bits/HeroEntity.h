#ifndef XY_HERO_ENTITY_H
#define XY_HERO_ENTITY_H

#include <gf/Entity.h>

#include "Hero.h"

namespace xy {
  struct GameData;
  struct GameState;

  class HeroEntity : public gf::Entity {
  public:
    HeroEntity(const GameState& state, Hero hero);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    const GameState& m_state;
    Hero m_hero;
  };

}

#endif // XY_HERO_ENTITY_H
