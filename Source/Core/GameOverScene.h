#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "axmol.h"

class GameOverScene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update
    };

public:
    bool init() override;
    void update(float delta) override;

    void setBackgroundScreenshot(ax::Sprite* screenshot);
    GameOverScene();
    ~GameOverScene();

private:
    GameState _gameState = GameState::init;
};

#endif  // __GAME_OVER_SCENE_H__