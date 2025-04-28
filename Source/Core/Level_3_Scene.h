#ifndef __LEVEL_3_SCENE_H__
#define __LEVEL_3_SCENE_H__

#include "axmol.h"

class Level_3_Scene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update,
    };

public:
    bool init() override;
    void update(float delta) override;

    Level_3_Scene();
    ~Level_3_Scene();

private:
    GameState _gameState = GameState::init;
};

#endif  // __LEVEL_3_SCENE_H__