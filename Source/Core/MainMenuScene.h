#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "axmol.h"

class MainMenuScene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update
    };

public:
    bool init() override;
    void update(float delta) override;

    MainMenuScene();
    ~MainMenuScene();

private:
    GameState _gameState = GameState::init;
};

#endif  // __MAIN_MENU_SCENE_H__