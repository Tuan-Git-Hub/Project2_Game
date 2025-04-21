#include "SceneManager.h"
#include "GameManager.h"
#include "SpriteManager.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"
#include "Level_1_Scene.h"
#include "TestTrapScene3.h"

using namespace ax;

SceneType SceneManager::_sceneType = SceneType::MainMenu_Scene;

Scene* SceneManager::createScene(SceneType type) 
{
    switch (type)
    {
        case SceneType::MainMenu_Scene:
            return utils::createInstance<MainMenuScene>();
        case SceneType::GameOver_Scene:
            return utils::createInstance<GameOverScene>();
        case SceneType::Level_1_Scene:
            return utils::createInstance<Level_1_Scene>();
        case SceneType::Level_3_Scene:
            return utils::createInstance<TestTrapScene3>();
        default:
            AXLOG("Error! SceneType in SceneManager is none");
            return nullptr;
    }
}

void SceneManager::create_and_replace_currentScene(SceneType type)
{
    auto replace_scene = createScene(type);
    if (!replace_scene)
        return;
    if (type == SceneType::GameOver_Scene)
    {
        auto gameoverScene = static_cast<GameOverScene*>(replace_scene);
        gameoverScene->setBackgroundScreenshot(SpriteManager::getInstance().getScreenshot(Director::getInstance()->getRunningScene()));
        Director::getInstance()->replaceScene(gameoverScene);
        return;
    }
    _sceneType = type;
    Director::getInstance()->replaceScene(replace_scene);
}

void SceneManager::restart_currentScene()
{
    GameManager::getInstance().resetGameManager();
    create_and_replace_currentScene(_sceneType);
}