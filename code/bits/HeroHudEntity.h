#ifndef RC_HERO_HUD_ENTITY_H
#define RC_HERO_HUD_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

#include "Hero.h"

namespace rc {

  class HeroHudEntity: public gf::Entity {
  public:
    HeroHudEntity(gf::ResourceManager& resources);

    void showInteract(Hero hero, bool showed);

    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

  private:
    gf::Font& m_font;

    std::vector<bool> m_showInteract;
  };

}

#endif // RC_HERO_HUD_ENTITY_H
