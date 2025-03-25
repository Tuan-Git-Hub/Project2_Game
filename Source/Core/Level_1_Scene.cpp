#include "Level_1_Scene.h"
#include "Utilities.h"
#include "UIManager.h"
#include "LevelManager.h"
#include "Player.h"
#include "CollisionManager.h"

using namespace ax;

static int s_sceneID = 2000;

bool Level_1_Scene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    //Tạo trọng lực
    getPhysicsWorld()->setGravity(Vec2(0, -900));
    //Bật hiển thị thông tin debug để kiểm tra va chạm
    //getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                           AX_CALLBACK_1(Level_1_Scene::menuCloseCallback, this));

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        Utilities::problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
        float y = safeOrigin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // Some templates (uncomment what you  need)
    _touchListener                 = EventListenerTouchAllAtOnce::create();
    _touchListener->onTouchesBegan = AX_CALLBACK_2(Level_1_Scene::onTouchesBegan, this);
    _touchListener->onTouchesMoved = AX_CALLBACK_2(Level_1_Scene::onTouchesMoved, this);
    _touchListener->onTouchesEnded = AX_CALLBACK_2(Level_1_Scene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
    /////////////////////////////
    // Hình background cho main menu scene
    auto background = Label::createWithTTF("BACK GROUND LEVEL 1", "fonts/Marker Felt.ttf", 50);
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 5 * 4 + origin.y));
    this->addChild(background, 1);
    
    // Tạo collision chung
    CollisionManager::init();



    // Tạo nút bấm
    auto buttons = UIManager::createMobileButtons();
    this->addChild(buttons);

    // Tạo map
    auto map = LevelManager::getInstance().loadLevel(LevelManager::Level::LEVEL_1);
    map->setPosition(Vec2(150, 170));
    this->addChild(map);

    // Tạo player
    auto player1 = Player::create();
    player1->setPosition(Vec2(250, 500));
    this->addChild(player1);


    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

void Level_1_Scene::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        // AXLOGD("onTouchesBegan detected, X:{}  Y:{}", t->getLocation().x, t->getLocation().y);
    }
}

void Level_1_Scene::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        // AXLOGD("onTouchesMoved detected, X:{}  Y:{}", t->getLocation().x, t->getLocation().y);
    }
}

void Level_1_Scene::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        // AXLOGD("onTouchesEnded detected, X:{}  Y:{}", t->getLocation().x, t->getLocation().y);
    }
}

void Level_1_Scene::update(float delta)
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


void Level_1_Scene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

Level_1_Scene::Level_1_Scene()
{
    _sceneID = ++s_sceneID;
    AXLOGD("Scene: ctor: #{}", _sceneID);
}

Level_1_Scene::~Level_1_Scene()
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