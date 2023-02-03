#ifndef XY_GAME_STATE_H
#define XY_GAME_STATE_H

#include <array>
#include <cstdint>

#include <gf/Path.h>

#include "HeroState.h"
#include "MapState.h"

namespace xy {

  struct DuplicatedState {
    HeroState hero;
    MapState map;
  };

  struct GameState {
    static constexpr uint16_t Version = 1;

    std::array<HeroState, 2> heros;
    HeroState& lisa = heros[0];
    HeroState& ryan = heros[1];

    std::array<MapState, 2> maps;
    MapState& lisaMap = maps[0];
    MapState& ryanMap = maps[1];

//     DuplicatedState& localHero(Hero hero);
//     DuplicatedState& otherHero(Hero hero);

    void loadFromFile(const gf::Path& filename);
    void saveToFile(const gf::Path& filename);
  };

  template<typename Archive>
  Archive& operator|(Archive& ar, GameState& state) {
    return ar;
  }

}


#endif // XY_GAME_STATE_H
