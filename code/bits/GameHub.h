#ifndef RC_GAME_HUB_H
#define RC_GAME_HUB_H

#include <gf/GameManager.h>

#include "AudioManager.h"
#include "CommonScene.h"
#include "GameState.h"
#include "GameOverScene.h"
#include "LiftScene.h"
#include "MainScene.h"
#include "StartScene.h"
#include "VictoryScene.h"

namespace rc {

  struct GameHub : gf::GameManager {
    GameHub();

    AudioManager audio;

    GameState state;

    CommonScene common;
    StartScene start;
    LiftScene lift;
    MainScene main;
    VictoryScene victory;
    GameOverScene gameOver;

  };


}


#endif // RC_GAME_HUB_H
