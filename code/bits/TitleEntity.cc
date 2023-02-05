#include "TitleEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace rc {

  TitleEntity::TitleEntity(gf::ResourceManager& resources)
  : m_fontTitle(resources.getFont("fonts/deportees.otf"))
  , m_fontSubTitle(resources.getFont("fonts/x-files.ttf"))
  {
  }

  void TitleEntity::update([[maybe_unused]] gf::Time time) {
  }

  void TitleEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.4f);

    gf::Text title("Root Company", m_fontTitle, titleCharacterSize);
    title.setColor(gf::Color::White);
    title.setPosition(coords.getCenter());
    title.setAnchor(gf::Anchor::BottomCenter);
    target.draw(title, states);

    unsigned subtitleCharacterSize = coords.getRelativeCharacterSize(0.05f);

    gf::Text subtitle("welcome in our laboratory about\n the culture of perennial plant", m_fontSubTitle, subtitleCharacterSize);
    subtitle.setColor(gf::Color::White);
    subtitle.setPosition(coords.getRelativePoint({ 0.0f, 0.6f }));
    subtitle.setParagraphWidth(coords.getRelativeSize({ 1.0f, 0.5f}).width);
    subtitle.setAlignment(gf::Alignment::Center);
    target.draw(subtitle, states);

    unsigned instructionsCharacterSize = coords.getRelativeCharacterSize(0.03f);

    gf::Text instructions("Press a button to start", m_fontSubTitle, instructionsCharacterSize);
    instructions.setColor(gf::Color::White);
    instructions.setPosition(coords.getRelativePoint({ 0.5f, 0.9f }));
    instructions.setAnchor(gf::Anchor::Center);
    target.draw(instructions, states);
  }

}
