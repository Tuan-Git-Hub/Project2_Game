#ifndef __LEVEL_4_SCENE_H__
#define __LEVEL_4_SCENE_H__

#include "axmol.h"

class Level_4_Scene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update,
    };

public:
    bool init() override;
    void update(float delta) override;

    Level_4_Scene();
    ~Level_4_Scene();

private:
    GameState _gameState = GameState::init;
};

#endif  // __LEVEL_4_SCENE_H__