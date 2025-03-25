#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "axmol.h"

class MainMenuScene : public ax::Scene
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

    MainMenuScene();
    ~MainMenuScene();

private:
    GameState _gameState                            = GameState::init;
    ax::EventListenerTouchAllAtOnce* _touchListener = nullptr;
    ax::EventListenerKeyboard* _keyboardListener    = nullptr;
    ax::EventListenerMouse* _mouseListener          = nullptr;
    int _sceneID = 0;
};

#endif  // __MAIN_MENU_SCENE_H__