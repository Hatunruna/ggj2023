#include "DialogEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace rc {

  namespace {
    constexpr gf::Time DialogCooldown = gf::seconds(5.f);
  }

  void DialogEntity::createDialogs() {
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "Hey Lisa, why are we supposed to be minions?"));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "We are not minions, Ryan, we have been assigned a very important task!"));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "We have to fetch a box from the archives. What a very important task!"));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "Listen, M. Cryant himself asked to take care of the task. It's not everyday that the big boss comes and talk to us!"));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "Well, basically, I am a laboratory assistant. I am not supposed to bring an archive to the boss. Moreover I am late for my latest report."));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "Me too, I am late on many cases."));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "He chose two among the latest to go deep in the building, in this rotten lift that takes minutes for each floor..."));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "Don't go too far, it's not minutes. But it's true that I feel uncomfortable."));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "..."));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "..."));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "By the way, what is in this box?"));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "I don't know. He just said that it was very important and we should not open it."));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "Great! At least, you know what it looks like?"));
    m_dialogs.push_back(std::make_pair(Hero::Lisa, "Well, yes. When we see it, we will know."));
    m_dialogs.push_back(std::make_pair(Hero::Ryan, "Very precise description. Ho, we finally arrived."));
  }


  DialogEntity::DialogEntity(gf::ResourceManager& resources)
  : m_font_text(resources.getFont("fonts/x-files.ttf"))
  {
    createDialogs();
  }

  void DialogEntity::update([[maybe_unused]] gf::Time time) {
    if (m_currentDialogIndex >= m_dialogs.size()) {
      return;
    }

    m_dialogCooldown += time;

    if (m_dialogCooldown > DialogCooldown) {
      m_dialogCooldown -= DialogCooldown;
      m_currentDialogIndex++;
    }
  }

  void DialogEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    if (m_currentDialogIndex >= m_dialogs.size()) {
      return;
    }

    gf::Coordinates coords(target);

    unsigned textCharacterSize = coords.getRelativeCharacterSize(0.05f);

    gf::Text character(m_dialogs.at(m_currentDialogIndex).first == Hero::Lisa ? "Lisa" : "Ryan", m_font_text, textCharacterSize);
    character.setColor(gf::Color::Gray());
    character.setPosition(coords.getRelativePoint({ 0.4f, 0.3f }));
    character.setAnchor(gf::Anchor::TopLeft);
    target.draw(character, states);

    gf::Text text(m_dialogs.at(m_currentDialogIndex).second, m_font_text, textCharacterSize);
    text.setParagraphWidth(coords.getRelativeSize({ 0.5f, 0.0f }).width);
    text.setAlignment(gf::Alignment::Left);
    text.setColor(gf::Color::White);
    text.setPosition(coords.getRelativePoint({ 0.4f, 0.4f }));
    text.setAnchor(gf::Anchor::TopLeft);
    target.draw(text, states);

    unsigned instructionsCharacterSize = coords.getRelativeCharacterSize(0.03f);

    gf::Text instructions("Press a button to start", m_font_text, instructionsCharacterSize);
    instructions.setParagraphWidth(coords.getRelativeSize({ 0.5f, 0.0f }).width);
    instructions.setAlignment(gf::Alignment::Center);
    instructions.setColor(gf::Color::Gray());
    instructions.setPosition(coords.getRelativePoint({ 0.4f, 0.9f }));
    instructions.setAnchor(gf::Anchor::TopLeft);
    target.draw(instructions, states);
  }

}
