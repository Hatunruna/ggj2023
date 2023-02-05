#include "RootEntity.h"

#include <gf/Log.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

#include "GameState.h"
#include "Settings.h"

namespace rc {

  namespace {
    constexpr int RootsBaseSize = 256;

    constexpr int RootsCount[] = {
      4, 4, 4, 4, 12, 4, 4, 4, 4, 12, 4, 4, 4, 4, 12, 0
    };

    gf::Vector2i computeRandomTile(bool north, bool east, bool south, bool west, bool tail, gf::Random& random) {
      int offset = (north ? 8 : 0) + (east ? 4 : 0) + (south ? 2 : 0) + (west ? 1 : 0) - 1;

      if (tail) {
        return gf::vec(15, offset);
      }

      return gf::vec(0, offset);
    }

    constexpr gf::Time RootUpdate = gf::seconds(1);

  }

  RootEntity::RootEntity(GameState& state, Hero hero, gf::ResourceManager& resources, gf::Random& random)
  : m_state(state)
  , m_hero(hero)
  , m_texture(resources.getTexture("images/roots.png"))
  , m_random(random)
  , m_time(RootUpdate)
  {
  }

  void RootEntity::update(gf::Time time) {
    const HeroState& localState = m_state.localPlayer(m_hero).hero;

    std::size_t levelIndex = localState.levelIndex;

    if (levelIndex >= m_state.roots.size()) {
      return;
    }

    RootState& root = m_state.roots[levelIndex];

    m_time += time;

    if (m_time >= RootUpdate) {
      m_time -= RootUpdate;

      root.head += gf::diry(1);
      root.parts.push_back(root.head);
      gf::Log::debug("Top!\n");
    }

  }

  void RootEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
    const HeroState& localState = m_state.localPlayer(m_hero).hero;

    std::size_t levelIndex = localState.levelIndex;

    if (levelIndex >= m_state.roots.size()) {
      return;
    }

    const RootState& root = m_state.roots[levelIndex];

    for (auto part : root.parts) {
      bool north = false;
      bool east = false;
      bool south = false;
      bool west = false;

      for (auto otherPart : root.parts) {
        if (part + gf::diry(-1) == otherPart) {
          north = true;
        }

        if (part + gf::dirx(1) == otherPart) {
          east = true;
        }

        if (part + gf::diry(1) == otherPart) {
          south = true;
        }

        if (part + gf::dirx(-1) == otherPart) {
          west = true;
        }
      }

      gf::RectI rect = gf::RectI::fromPositionSize(
        computeRandomTile(north, east, south, west, part == root.tail, m_random) * RootsBaseSize,
        gf::vec(RootsBaseSize, RootsBaseSize)
      );

      gf::Sprite sprite(m_texture, m_texture.computeTextureCoords(rect));
      sprite.setPosition(part * CellSize);
      sprite.setScale(CellSize / SpriteSize);
      target.draw(sprite, states);
    }

  }

}
