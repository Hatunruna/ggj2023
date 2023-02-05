#ifndef RC_GAME_HUB_H
#define RC_GAME_HUB_H

#include <gf/GameManager.h>

#include "CommonScene.h"
#include "GameState.h"
#include "LiftScene.h"
#include "MainScene.h"
#include "StartScene.h"

namespace rc {

  struct GameHub : gf::GameManager {
    GameHub();

    GameState state;

    CommonScene common;
    StartScene start;
    MainScene main;
    LiftScene lift;
  };


}


#endif // RC_GAME_HUB_H
