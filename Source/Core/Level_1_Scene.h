#ifndef __LEVEL_1_SCENE_H__
#define __LEVEL_1_SCENE_H__

#include "axmol.h"

class Level_1_Scene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update,
        pause,
        end,
        menu1,
        menu2,
    };

public:
    bool init() override;
    void update(float delta) override;

    // touch
    void onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event);

    // a selector callback
    void menuCloseCallback(ax::Object* sender);

    // gameover
    void gameOver();

    Level_1_Scene();
    ~Level_1_Scene();

private:
    GameState _gameState                            = GameState::init;
    ax::EventListenerTouchAllAtOnce* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener    = nullptr;
    ax::EventListenerMouse* _mouseListener          = nullptr;
    int _sceneID = 0;
};


#endif  // __LEVEL_1_SCENE_H__