#include "DialogEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace rc {

  namespace {
    constexpr gf::Time DialogCooldown = gf::seconds(2.f);
  }

  void DialogEntity::createDialogs() {
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "1text text ?"));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "2text textlezjds ?"));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "3text ted;jbdklkcndsjbcknl,c: xt ?"));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "4text tcs;j wnksf;jsrbd:vext ?"));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "5text textcshv kbqldkcd ?"));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "6text sdlkcnlq text ?"));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "7text dbjs ksjdbck dtext ?"));
  }


  DialogEntity::DialogEntity(gf::ResourceManager& resources)
  : m_font_text(resources.getFont("fonts/x-files.ttf"))
//   , m_backgroundTexture(resources.getTexture("logo.png"))
  {
    createDialogs();
  }

  void DialogEntity::update([[maybe_unused]] gf::Time time) {
    m_dialogCooldown += time;
    if (m_dialogCooldown > DialogCooldown) {
      m_dialogCooldown -= DialogCooldown;
      if (m_currentDialogIndex + 1 == m_dialogs.size()) {
        return;
      }
      m_currentDialogIndex++;
    }
  }

  void DialogEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    unsigned textCharacterSize = coords.getRelativeCharacterSize(0.05f);

    gf::Text text(m_dialogs.at(m_currentDialogIndex).second, m_font_text, textCharacterSize);
    text.setColor(gf::Color::White);
    text.setPosition(coords.getRelativePoint({1.f,0.f}));
    text.setAnchor(gf::Anchor::TopRight);
    target.draw(text, states);

    unsigned instructionsCharacterSize = coords.getRelativeCharacterSize(0.03f);

    /*gf::Text instructions("Press a button to start", m_font_sub_title, instructionsCharacterSize);
    instructions.setColor(gf::Color::White);
    instructions.setPosition(coords.getRelativePoint({ 0.5f, 0.9f }));
    instructions.setAnchor(gf::Anchor::Center);
    target.draw(instructions, states);*/
  }

}
