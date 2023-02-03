#ifndef XY_GAME_HUB_H
#define XY_GAME_HUB_H

#include <gf/GameManager.h>

#include "GameState.h"
#include "MainScene.h"
#include "StartScene.h"

namespace xy {

  struct GameHub : gf::GameManager {
    GameHub();

    GameState state;

    StartScene start;
    MainScene main;
  };


}


#endif // XY_GAME_HUB_H
