#ifndef XY_HERO_ENTITY_H
#define XY_HERO_ENTITY_H

#include <gf/Entity.h>
#include <gf/Direction.h>

#include "Hero.h"

namespace xy {
  struct GameData;
  struct GameState;

  class HeroEntity : public gf::Entity {
  public:
    HeroEntity(const GameData& data, GameState& state, Hero hero);

    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
    void move(gf::Direction dir);

  private:
    const GameData& m_data;
    GameState& m_state;
    Hero m_hero;
    gf::Time m_moveCooldown;
    gf::Direction m_currentDirection;
    bool stillRuning;
  };

}

#endif // XY_HERO_ENTITY_H
