#ifndef RC_TITLE_ENTITY_H
#define RC_TITLE_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>

namespace rc {

  class TitleEntity : public gf::Entity {
  public:
    TitleEntity(gf::ResourceManager& resources);
    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
  private:
    gf::Font& m_font_title;
    gf::Font& m_font_sub_title;
//     gf::Texture& m_backgroundTexture;
  };


}


#endif // RC_TITLE_ENTITY_H
