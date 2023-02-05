#include "TitleEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace rc {

  TitleEntity::TitleEntity(gf::ResourceManager& resources)
  : m_font_title(resources.getFont("deportees/Deportees.otf"))
  , m_font_sub_title(resources.getFont("xfiles1/x-files.ttf"))
//   , m_backgroundTexture(resources.getTexture("logo.png"))
  {
  }

  void TitleEntity::update([[maybe_unused]] gf::Time time) {
  }

  void TitleEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

//     float backgroundHeight = coords.getRelativeSize(gf::vec(0.0f, 0.8f)).height;
//     float backgroundScale = backgroundHeight / m_backgroundTexture.getSize().height;
//
//     gf::Sprite background(m_backgroundTexture);
//     background.setColor(gf::Color::Opaque(0.25));
//     background.setPosition(coords.getCenter());
//     background.setAnchor(gf::Anchor::Center);
//     background.setScale(backgroundScale);
//     target.draw(background, states);

    unsigned titleCharacterSize = coords.getRelativeCharacterSize(0.4f);

    gf::Text title("Root Company", m_font_title, titleCharacterSize);
    title.setColor(gf::Color::White);
    title.setPosition(coords.getCenter());
    title.setAnchor(gf::Anchor::BottomCenter);
    target.draw(title, states);

    unsigned subtitleCharacterSize = coords.getRelativeCharacterSize(0.05f);

    gf::Text subtitle("welcome in our laboratory about\n the culture of perennial plant", m_font_sub_title, subtitleCharacterSize);
    subtitle.setColor(gf::Color::White);
    subtitle.setPosition(coords.getRelativePoint({ 0.0f, 0.6f }));
    //subtitle.setAnchor(gf::Anchor::Center);
    subtitle.setParagraphWidth(coords.getRelativeSize({ 1.0f, 0.5f}).width);
    subtitle.setAlignment(gf::Alignment::Center);
    target.draw(subtitle, states);

    unsigned instructionsCharacterSize = coords.getRelativeCharacterSize(0.03f);

    gf::Text instructions("Press a button to start", m_font_sub_title, instructionsCharacterSize);
    instructions.setColor(gf::Color::White);
    instructions.setPosition(coords.getRelativePoint({ 0.5f, 0.9f }));
    instructions.setAnchor(gf::Anchor::Center);
    target.draw(instructions, states);
  }

}
