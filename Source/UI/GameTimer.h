#ifndef __GAME_TIMER_H__
#define __GAME_TIMER_H__

#include "axmol.h"

class GameTimer : public ax::Node
{
private:
    ax::Label* timeLabel;
    float timeLeft;
public:
    static GameTimer* createGameTimer();
    bool init();
    void drawTimeLeft(float t);
};

#endif  // __GAME_TIMER_H__