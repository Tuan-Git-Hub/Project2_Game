#include "MainMenuScene.h"
#include "Utilities.h"
#include "UIManager.h"
#include "GameObjectManager.h"
#include "GameManager.h"


using namespace ax;

static int s_sceneID = 1000;

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
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
    // Some templates (uncomment what you  need)
    _touchListener                 = EventListenerTouchAllAtOnce::create();
    _touchListener->onTouchesBegan = AX_CALLBACK_2(MainMenuScene::onTouchesBegan, this);
    _touchListener->onTouchesMoved = AX_CALLBACK_2(MainMenuScene::onTouchesMoved, this);
    _touchListener->onTouchesEnded = AX_CALLBACK_2(MainMenuScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    /////////////////////////////
    // Load texture và sprite frame 1 lượt
    UIManager::loadTextures_SpriteFrames();
    GameObjectManager::loadTextures_SpriteFrames();
    // Hình background cho main menu scene
    auto background = UIManager::createBg();
    this->addChild(background);

    // Bảng menu
    auto menuBoard = UIManager::createMainMenu();
    this->addChild(menuBoard, 1);

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

void MainMenuScene::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        // AXLOGD("onTouchesBegan detected, X:{}  Y:{}", t->getLocation().x, t->getLocation().y);
    }
}

void MainMenuScene::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        // AXLOGD("onTouchesMoved detected, X:{}  Y:{}", t->getLocation().x, t->getLocation().y);
    }
}

void MainMenuScene::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        // AXLOGD("onTouchesEnded detected, X:{}  Y:{}", t->getLocation().x, t->getLocation().y);
    }
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
        /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;
    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()

        break;
    }

    case GameState::menu1:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu1();
        break;
    }

    case GameState::menu2:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu2();
        break;
    }

    case GameState::end:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }

    }  // switch
}


void MainMenuScene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

MainMenuScene::MainMenuScene()
{
    _sceneID = ++s_sceneID;
    AXLOGD("Scene: ctor: #{}", _sceneID);
}

MainMenuScene::~MainMenuScene()
{
    AXLOGD("~Scene: dtor: #{}", _sceneID);

    if (_touchListener)
        _eventDispatcher->removeEventListener(_touchListener);
    if (_keyboardListener)
        _eventDispatcher->removeEventListener(_keyboardListener);
    if (_mouseListener)
        _eventDispatcher->removeEventListener(_mouseListener);
    _sceneID = -1;
}