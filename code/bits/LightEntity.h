#ifndef RC_LIGHT_ENTITY_H
#define RC_LIGHT_ENTITY_H

#include <gf/Entity.h>

namespace rc {

  struct GameState;

  class LightEntity : public gf::Entity {
  public:
    LightEntity(GameState& state);

    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    const GameState& m_state;
  };

}


#endif // RC_LIGHT_ENTITY_H
