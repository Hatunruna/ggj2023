#ifndef XY_HERO_H
#define XY_HERO_H

namespace xy {

  enum class Hero : int {
    Lisa  = 0,
    Ryan  = 1,
  };

  int getOtherHeroIndex(Hero hero);

}

#endif // XY_HERO_H
