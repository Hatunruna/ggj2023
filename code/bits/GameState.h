#ifndef RC_GAME_STATE_H
#define RC_GAME_STATE_H

#include <cassert>
#include <cstdint>
#include <array>

#include <gf/Path.h>

#include "Hero.h"
#include "HeroState.h"
#include "MapState.h"
#include "RootState.h"

namespace rc {

  enum class GameStatus {
    Playing,
    Victory,
    GameOver,
  };

  enum class GamePhase {
    Light,
    Dark,
  };

  struct PlayerState {
    HeroState hero;
    MapState map;
  };

  struct GameState {
    static constexpr uint16_t Version = 1;

    GameStatus status = GameStatus::Playing;
    GamePhase phase = GamePhase::Light;

    std::array<PlayerState, 2> players;
    PlayerState& lisa = players[0];
    PlayerState& ryan = players[1];

    std::vector<RootState> roots;

    PlayerState& localPlayer(Hero hero) {
      switch (hero) {
        case Hero::Lisa:
          return lisa;
        case Hero::Ryan:
          return ryan;
      }

      assert(false);
      return lisa;
    }

    const PlayerState& localPlayer(Hero hero) const {
      switch (hero) {
        case Hero::Lisa:
          return lisa;
        case Hero::Ryan:
          return ryan;
      }

      assert(false);
      return lisa;
    }

    PlayerState& otherPlayer(Hero hero) {
      switch (hero) {
        case Hero::Lisa:
          return ryan;
        case Hero::Ryan:
          return lisa;
      }

      assert(false);
      return lisa;
    }

    const PlayerState& otherPlayer(Hero hero) const {
      switch (hero) {
        case Hero::Lisa:
          return ryan;
        case Hero::Ryan:
          return lisa;
      }

      assert(false);
      return lisa;
    }

    void loadFromFile(const gf::Path& filename);
    void saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameState& state) {
    return ar;
  }

}


#endif // RC_GAME_STATE_H
