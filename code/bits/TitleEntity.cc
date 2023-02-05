#include "TitleEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace rc {

  TitleEntity::TitleEntity(gf::ResourceManager& resources)
  : m_fontSubTitle(resources.getFont("fonts/x-files.ttf"))
  , m_texture(resources.getTexture("images/title.jpg"))
  {
  }

  void TitleEntity::update([[maybe_unused]] gf::Time time) {
  }

  void TitleEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    gf::Sprite background;
    background.setTexture(m_texture);
    background.setScale(coords.getWindowSize() / gf::vec(1920.0f, 1080.0f));
    target.draw(background, states);

    unsigned subtitleCharacterSize = coords.getRelativeCharacterSize(0.05f);

    gf::Text subtitle("welcome in our laboratory about\n the culture of perennial plant", m_fontSubTitle, subtitleCharacterSize);
    subtitle.setColor(gf::Color::White);
    subtitle.setPosition(coords.getRelativePoint({ 0.0f, 0.65f }));
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
