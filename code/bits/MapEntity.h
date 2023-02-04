#ifndef XY_MAP_ENTITY_H
#define XY_MAP_ENTITY_H

#include <gf/Entity.h>

#include "Hero.h"

namespace xy {

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

#endif // XY_MAP_ENTITY_H
