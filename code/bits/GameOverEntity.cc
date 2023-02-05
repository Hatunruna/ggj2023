#include "GameOverEntity.h"

#include <gf/Coordinates.h>
#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Text.h>

namespace rc {

  GameOverEntity::GameOverEntity(gf::ResourceManager& resources)
  : m_texture(resources.getTexture("images/title.jpg"))
  {
  }

  void GameOverEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    gf::Coordinates coords(target);

    gf::Sprite background;
    background.setTexture(m_texture);
    background.setScale(coords.getWindowSize() / gf::vec(1920.0f, 1080.0f));
    target.draw(background, states);
  }

}
