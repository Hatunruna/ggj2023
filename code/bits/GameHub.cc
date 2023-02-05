#include "GameHub.h"

#include "config.h"

namespace rc {

  GameHub::GameHub()
  : GameManager("Root Company", { GAME_RELATIVE_DATADIR, GAME_DATADIR })
  , common(*this)
  , start(*this)
  , lift(*this)
  , main(*this)
  , victory(*this)
  , gameOver(*this)
  {
    gf::Ref<gf::Scene> scenes[] = { common, start };
    pushScenes(scenes);
  }

}
