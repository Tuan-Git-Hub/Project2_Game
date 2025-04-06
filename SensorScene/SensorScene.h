#ifndef __SENSOR_SCENE_H__
#define __SENSOR_SCENE_H__

#include "axmol.h"

class SensorScene : public ax::Scene
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

    static ax::Scene* createScene();

    bool onContactBegin(ax::PhysicsContact& contact);
    bool onContactSeparate(ax::PhysicsContact& contact);

    // touch
    void onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event);
    void onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event);

    // Keyboard
    void onKeyPressed(ax::EventKeyboard::KeyCode code, ax::Event* event);
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event);

    // a selector callback
    void menuCloseCallback(ax::Object* sender);

private:
    GameState _gameState = GameState::init;
};

#endif  // __SENSOR_SCENE_H__
