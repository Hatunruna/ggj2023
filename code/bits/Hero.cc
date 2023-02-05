#include "Hero.h"

#include <cassert>

namespace rc {
  int getOtherHeroIndex(Hero hero) {
    switch (hero) {
      case Hero::Lisa:
        return static_cast<int>(Hero::Ryan);

      case Hero::Ryan:
        return static_cast<int>(Hero::Lisa);
    }

    assert(false);
    return -1;
  }
}
