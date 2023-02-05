#include "RootEntity.h"

#include <cassert>

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "GameState.h"
#include "Settings.h"

namespace rc {

  namespace {
    constexpr int RootsBaseSize = 256;
  }

  RootEntity::RootEntity(GameState& state, Hero hero, gf::ResourceManager& resources)
  : m_state(state)
  , m_hero(hero)
  , m_texture(resources.getTexture("images/roots.png"))
  {
  }

  void RootEntity::update(gf::Time time) {
  }

  void RootEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    const HeroState& localState = m_state.localPlayer(m_hero).hero;

    std::size_t levelIndex = localState.levelIndex;

    if (levelIndex >= m_state.roots.size()) {
      return;
    }

    const RootState& root = m_state.roots[levelIndex];

    for (auto & part : root.parts) {
      gf::RectI rect = gf::RectI::fromPositionSize(
        part.tile * RootsBaseSize,
        gf::vec(RootsBaseSize, RootsBaseSize)
      );

      gf::Sprite sprite(m_texture, m_texture.computeTextureCoords(rect));
      sprite.setPosition(part.position * CellSize);
      sprite.setScale(CellSize / SpriteSize);
      target.draw(sprite, states);
    }

  }

}
