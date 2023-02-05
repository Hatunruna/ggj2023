#include "RootModel.h"

#include <gf/Log.h>

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
    m_time += time;

    if (m_time < RootUpdate) {
      return;
    }

    m_time -= RootUpdate;

    std::size_t lisaIndex = m_state.lisa.hero.levelIndex;
    auto lisaTarget = findNextStep(lisaIndex, Hero::Lisa);

    std::size_t ryanIndex = m_state.ryan.hero.levelIndex;
    auto ryanTarget = findNextStep(ryanIndex, Hero::Ryan);

    if (lisaIndex == ryanIndex) {
      if (lisaTarget && ryanTarget) {
        if (std::get<1>(*lisaTarget) < std::get<1>(*ryanTarget)) {
          updateLevel(lisaIndex, std::get<0>(*lisaTarget));
        } else {
          updateLevel(ryanIndex, std::get<0>(*ryanTarget));
        }
      } else if (lisaTarget) {
        updateLevel(lisaIndex, std::get<0>(*lisaTarget));
      } else if (ryanTarget) {
        updateLevel(ryanIndex, std::get<0>(*ryanTarget));
      }
    } else {
      if (lisaTarget) {
        updateLevel(lisaIndex, std::get<0>(*lisaTarget));
      }
      if (ryanTarget) {
        updateLevel(ryanIndex, std::get<0>(*ryanTarget));
      }
    }
  }

  std::optional<std::tuple<gf::Vector2i, std::size_t>> RootModel::findNextStep(std::size_t levelIndex, Hero target) {
    if (levelIndex >= m_state.roots.size()) {
//      assert(false);
      return std::nullopt;
    }

    RootState& root = m_state.roots[levelIndex];
    PlayerState& player = m_state.localPlayer(target);

    auto path = player.map.levels[levelIndex].map.computeRoute(root.head, player.hero.position, 0.0f);

    if (path.size() < 2) {
      return std::nullopt;
    }

    return std::make_tuple(path[1], path.size());
  }

  void RootModel::updateLevel(std::size_t levelIndex, gf::Vector2i next) {
    if (levelIndex >= m_state.roots.size()) {
      // TODO: make the root grow up to this level
      return;
    }

    RootState& root = m_state.roots[levelIndex];

    gf::Log::debug("Head: %i, %i, Next: %i,%i\n", root.head.x, root.head.y, next.x, next.y);

    root.head = next;
    root.parts.push_back({ root.head, gf::vec(0, 0) });
    updateRoot(root);
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
