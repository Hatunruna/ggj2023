#ifndef RC_GAME_OVER_ENTITY_H
#define RC_GAME_OVER_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>
#include <gf/Texture.h>

namespace rc {

  class GameOverEntity : public gf::Entity {
  public:
    GameOverEntity(gf::ResourceManager& resources);
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
  private:
    gf::Texture& m_texture;
  };


}


#endif // RC_GAME_OVER_ENTITY_H
