#include "RootModel.h"

#include <gf/Geometry.h>
#include <gf/Log.h>

#include "GameState.h"

namespace rc {

  namespace {

    constexpr gf::Time RootUpdate = gf::seconds(0.1f);

    constexpr int RootsCount[] = {
      4, 4, 4, 4, 12, 4, 4, 4, 4, 12, 4, 4, 4, 4, 8, 0
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
  , m_lightTime(gf::Time::zero())
  , m_lightLimit(gf::seconds(1.0f))
  , m_time(RootUpdate)
  {
  }

  void RootModel::update(gf::Time time) {
    if (m_state.phase == GamePhase::Light) {
      m_lightTime += time;

      if (m_lightTime > m_lightLimit) {
        m_state.phase = GamePhase::Dark;
        m_darkTime = gf::Time::zero();
        m_darkLimit = gf::seconds(m_random.computeUniformFloat(0.05f, 0.3f));
      }

    } else {
      m_darkTime += time;

      if (m_darkTime > m_darkLimit) {
        m_state.phase = GamePhase::Light;
        m_lightTime = gf::Time::zero();

        if (m_random.computeBernoulli(0.3f)) {
          m_lightLimit = gf::seconds(m_random.computeUniformFloat(5.0f, 8.0f));
        } else {
          m_lightLimit = gf::seconds(m_random.computeUniformFloat(0.05f, 0.3f));
        }
      }
    }

    if (m_state.phase == GamePhase::Light) {
      time = gf::seconds(time.asSeconds() * 0.05f);
    }

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
    PlayerState& player = m_state.localPlayer(target);

    while (levelIndex >= m_state.roots.size()) {
      gf::Vector2i current = m_state.roots.back().head;

      gf::Bresenham bresenham(current, player.hero.position);

      do {
        if (m_state.lisa.map.levels[m_state.roots.size()].map.isWalkable(current)) {
          break;
        }
      } while (bresenham.step(current));


      RootState root;
      root.head = root.tail = current;
      root.parts.push_back({ current, gf::vec(0, 15) });

      m_state.roots.push_back(std::move(root));
    }

    RootState& root = m_state.roots[levelIndex];

    if (root.head == player.hero.position) {
      m_state.status = GameStatus::GameOver;
      return std::nullopt;
    }

    auto path = player.map.levels[levelIndex].map.computeRoute(root.head, player.hero.position, 0.0f);

    if (path.size() < 2) {
      return std::nullopt;
    }

    return std::make_tuple(path[1], path.size());
  }

  void RootModel::updateLevel(std::size_t levelIndex, gf::Vector2i next) {
    assert(levelIndex < m_state.roots.size());

    RootState& root = m_state.roots[levelIndex];
    root.head = next;
    root.parts.push_back({ root.head, gf::vec(0, 0) });
    updateRoot(root);

    m_state.lisa.map.levels[levelIndex].map.setWalkable(next, false);
    m_state.ryan.map.levels[levelIndex].map.setWalkable(next, false);
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
