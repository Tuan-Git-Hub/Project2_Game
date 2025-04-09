#ifndef __GAME_TIMER_H__
#define __GAME_TIMER_H__

#include "axmol.h"

class GameTimer : public ax::Node
{
private:
    ax::Label* timeLabel;
    float timeLeft;
    float time_level_1 = 15.0f;
    float time_level_2 = 100.0f;
    float time_level_3 = 100.0f;
public:
    enum class Level
    {
        LEVEL_1,
        LEVEL_2,
        LEVEL_3
    };
    static GameTimer* createGameTimer(Level level);
    bool init(Level level);
    std::function<void()> timeOut;
    void update(float dt);
};

#endif  // __GAME_TIMER_H__