#ifndef __LEVEL_2_SCENE_H__
#define __LEVEL_2_SCENE_H__

#include "axmol.h"

class Level_2_Scene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update,
    };

public:
    bool init() override;
    void update(float delta) override;

    Level_2_Scene();
    ~Level_2_Scene();

private:
    GameState _gameState = GameState::init;
};

#endif  // __LEVEL_2_SCENE_H__