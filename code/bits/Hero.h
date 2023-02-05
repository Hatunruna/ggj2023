#ifndef RC_HERO_H
#define RC_HERO_H

namespace rc {

  enum class Hero : int {
    Lisa  = 0,
    Ryan  = 1,
  };

  int getOtherHeroIndex(Hero hero);

}

#endif // RC_HERO_H
