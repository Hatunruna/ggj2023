#ifndef RC_DIALOG_ENTITY_H
#define RC_DIALOG_ENTITY_H

#include <gf/Entity.h>
#include <gf/Font.h>
#include <gf/ResourceManager.h>
#include <gf/Time.h>

#include "Hero.h"

namespace rc {

  class DialogEntity : public gf::Entity {
  public:
    DialogEntity(gf::ResourceManager& resources);
    void update(gf::Time time) override;
    void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
  private:
    void createDialogs();
    gf::Font& m_font_text;
//     gf::Texture& m_backgroundTexture;
    std::vector<std::pair<Hero, std::string>> m_dialogs;
    int m_currentDialogIndex = 0;
    gf::Time m_dialogCooldown = gf::seconds(0);

  };


}


#endif // RC_DIALOG_ENTITY_H
