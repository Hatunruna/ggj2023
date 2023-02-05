#ifndef RC_MAP_ENTITY_H
#define RC_MAP_ENTITY_H

#include <gf/Entity.h>

#include "Hero.h"

namespace rc {

  struct GameHub;
  struct GameState;

  class MapEntity : public gf::Entity {
  public:
    MapEntity(GameHub& game, GameState& state, Hero hero);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    GameState& m_state;
    Hero m_hero;
  };

}

#endif // RC_MAP_ENTITY_H
