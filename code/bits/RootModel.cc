#include "RootModel.h"

#include "GameState.h"

namespace rc {

  namespace {

    constexpr gf::Time RootUpdate = gf::seconds(1);

    constexpr int RootsCount[] = {
      4, 4, 4, 4, 12, 4, 4, 4, 4, 12, 4, 4, 4, 4, 12, 0
    };

    gf::Vector2i computeRandomTile(bool north, bool east, bool south, bool west, bool tail, gf::Random& random) {
      int offset = (north ? 8 : 0) + (east ? 4 : 0) + (south ? 2 : 0) + (west ? 1 : 0) - 1;
      assert(0 <= offset && offset <= 14);

      if (tail) {
        return gf::vec(15, offset);
      }

      int which = random.computeUniformInteger(0, RootsCount[offset] - 1);
      return gf::vec(which, offset);
    }

  }

  RootModel::RootModel(GameState& state, gf::Random& random)
  : m_state(state)
  , m_random(random)
  , m_time(RootUpdate)
  {
  }

  void RootModel::update(gf::Time time) {
    std::size_t lisaIndex = m_state.lisa.hero.levelIndex;
    std::size_t ryanIndex = m_state.ryan.hero.levelIndex;

    updateLevel(time, lisaIndex);

    if (lisaIndex != ryanIndex) {
      updateLevel(time, ryanIndex);
    }
  }

  void RootModel::updateLevel(gf::Time time, std::size_t levelIndex) {
    if (levelIndex >= m_state.roots.size()) {
      // TODO: make the root grow up to this level
      return;
    }

    RootState& root = m_state.roots[levelIndex];

    m_time += time;

    if (m_time >= RootUpdate) {
      m_time -= RootUpdate;

      // TODO: search for a hero
      root.head += gf::diry(1);
      root.parts.push_back({ root.head, gf::vec(0, 0) });

      updateRoot(root);
    }
  }

  void RootModel::updateRoot(RootState& root) {
    for (auto & part : root.parts) {
      bool north = false;
      bool east = false;
      bool south = false;
      bool west = false;

      for (auto otherPart : root.parts) {
        if (part.position + gf::diry(-1) == otherPart.position) {
          north = true;
        }

        if (part.position + gf::dirx(1) == otherPart.position) {
          east = true;
        }

        if (part.position + gf::diry(1) == otherPart.position) {
          south = true;
        }

        if (part.position + gf::dirx(-1) == otherPart.position) {
          west = true;
        }
      }

      part.tile = computeRandomTile(north, east, south, west, part.position == root.tail, m_random);
    }

  }

}
