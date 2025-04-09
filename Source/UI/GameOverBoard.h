#ifndef __GAME_OVER_BOARD_H__
#define __GAME_OVER_BOARD_H__

#include "axmol.h"

class GameOverBoard : public ax::Layer
{
public:
    static GameOverBoard* createGameOverBoard();
    bool init();

    void onRestart();
    void onReturnMainMenu();

};



#endif  // __GAME_OVER_BOARD_H__