#include "GameHub.h"

#include "config.h"

namespace rc {

  GameHub::GameHub()
  : GameManager("Root Company", { GAME_RELATIVE_DATADIR, GAME_DATADIR })
  , common(*this)
  , start(*this)
  , main(*this)
  , lift(*this)
  {
    gf::Ref<gf::Scene> scenes[] = { common, start };
    pushScenes(scenes);
  }

}
