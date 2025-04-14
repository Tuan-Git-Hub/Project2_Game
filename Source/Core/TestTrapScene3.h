#ifndef __TEST_TRAP_SCENE3_H__
#define __TEST_TRAP_SCENE3_H__

#include "axmol.h"

class TestTrapScene3 : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update,
        end,
    };

public:
    bool init() override;
    void update(float delta) override;


    TestTrapScene3();
    ~TestTrapScene3();

private:
    GameState _gameState = GameState::init;
};


#endif  // __TEST_TRAP_SCENE3_H__