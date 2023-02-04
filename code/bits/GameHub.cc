#include "GameHub.h"

#include "config.h"

namespace xy {

  GameHub::GameHub()
  : GameManager("Game", { GAME_RELATIVE_DATADIR, GAME_DATADIR })
  , common(*this)
  , start(*this)
  , main(*this)
  {
    gf::Ref<gf::Scene> scenes[] = { common, start };
    pushScenes(scenes);
  }

}
