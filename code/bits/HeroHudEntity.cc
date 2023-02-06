#include "HeroHudEntity.h"

#include <gf/Coordinates.h>
#include <gf/RenderTarget.h>
#include <gf/Text.h>

namespace rc {

  HeroHudEntity::HeroHudEntity(gf::ResourceManager& resources)
  : m_font(resources.getFont("fonts/x-files.ttf"))
  , m_showInteract({ false, false })
  {

  }

  void HeroHudEntity::showInteract(Hero hero, bool showed) {
    m_showInteract[static_cast<int>(hero)] = showed;
  }

  void HeroHudEntity::update(gf::Time time) {

  }

  void HeroHudEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    for (int i = 0; i < m_showInteract.size(); ++i) {
      if (m_showInteract[i]) {
        gf::Text interactText;

        switch (static_cast<Hero>(i)) {
          case Hero::Lisa:
            interactText.setString("SPACE to use the computer");
            interactText.setPosition(coords.getRelativePoint(gf::vec(0.25f, 0.40f)));
            break;

          case Hero::Ryan:
            interactText.setString(" to use the computer");
            interactText.setPosition(coords.getRelativePoint(gf::vec(0.76f, 0.40f)));
            break;
        }

        interactText.setFont(m_font);
        interactText.setCharacterSize(coords.getRelativeCharacterSize(0.025f));
        interactText.setColor(gf::Color::White);
        interactText.setOutlineColor(gf::Color::Black);
        interactText.setOutlineThickness(3.0f);
        interactText.setAnchor(gf::Anchor::Center);

        target.draw(interactText, states);

        if (i == static_cast<int>(Hero::Ryan)) {
          gf::RectF textBounds = interactText.getLocalBounds();

          // Gamepad 'icon'
          interactText.setString("A");
          interactText.setPosition(coords.getRelativePoint(gf::vec(0.76f, 0.40f)) - gf::dirx(textBounds.getWidth() * 0.5f));
          interactText.setFont(m_font);
          interactText.setCharacterSize(coords.getRelativeCharacterSize(0.025f));
          interactText.setColor(gf::Color::Green);
          interactText.setOutlineColor(gf::Color::Black);
          interactText.setOutlineThickness(3.0f);
          interactText.setAnchor(gf::Anchor::CenterRight);
          target.draw(interactText, states);
        }
      }
    }
  }

}
