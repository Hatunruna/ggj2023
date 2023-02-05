#ifndef RC_VICTORY_ENTITY_H
#define RC_VICTORY_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>

namespace rc {

  class VictoryEntity : public gf::Entity {
  public:
    VictoryEntity(gf::ResourceManager& resources);
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
  private:
    gf::Texture& m_texture;
  };


}


#endif // RC_VICTORY_ENTITY_H
