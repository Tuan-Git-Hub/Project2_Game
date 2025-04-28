#include "MainMenuScene.h"
#include "Utilities.h"
#include "UIManager.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "SoundManager.h"


using namespace ax;

bool MainMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    // Tạo hoặc reset game manager để tạo logic game
    GameManager::getInstance().resetGameManager();
    // Tạo âm thanh
    SoundManager::playBackground(AudioPaths::MAINMENU_BACKGROUND, true);
    
    // Load texture và sprite frame 1 lượt
    UIManager::loadTextures_SpriteFrames();
    GameObjectManager::loadTextures_SpriteFrames();
    // Hình background cho main menu scene
    auto background = UIManager::createBg();
    this->addChild(background);

    // Bảng menu
    auto menuBoard = UIManager::createMainMenu();
    this->addChild(menuBoard, 1);

    scheduleUpdate();

    return true;
}

void MainMenuScene::update(float delta)
{
    switch (_gameState)
    {
        case GameState::init:
        {
            _gameState = GameState::update;
            break;
        }

        case GameState::update:
        {
            break;
        }
    }
}

MainMenuScene::MainMenuScene()
{
    AXLOGD("Scene: Main Menu Scene ctor");
}

MainMenuScene::~MainMenuScene()
{
    AXLOGD("~Scene: Main Menu Scene dtor");
}