#include "GameHub.h"

#include "config.h"

namespace xy {

  GameHub::GameHub()
  : GameManager("Game", { GAME_RELATIVE_DATADIR, GAME_DATADIR })
  , start(*this)
  , main(*this)
  {
    pushScene(start);
  }

}
