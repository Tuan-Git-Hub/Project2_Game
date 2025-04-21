#ifndef __LEVEL_1_SCENE_H__
#define __LEVEL_1_SCENE_H__

#include "axmol.h"

class Level_1_Scene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update,
    };

public:
    bool init() override;
    void update(float delta) override;

    Level_1_Scene();
    ~Level_1_Scene();

private:
    GameState _gameState = GameState::init;
};

#endif  // __LEVEL_1_SCENE_H__