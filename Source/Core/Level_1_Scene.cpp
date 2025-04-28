#include "Level_1_Scene.h"
#include "Utilities.h"
#include "UIManager.h"
#include "LevelManager.h"
#include "GameManager.h"
#include "CollisionManager.h"
#include "CameraController.h"
#include "SoundManager.h"
#include "GameObjectManager.h"

using namespace ax;

bool Level_1_Scene::init()
{
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    //Tạo trọng lực
    getPhysicsWorld()->setGravity(Vec2(0, -980));
    
    //this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    // Chọn level để game manager tạo logic game
    GameManager::getInstance().selectLevel(GameManager::Level::LEVEL_1);
    // Tạo âm thanh
    SoundManager::playBackground(AudioPaths::LEVEL1_BACKGROUND, true);
    // Tạo map
    auto map = LevelManager::loadLevel(Level::LEVEL_1);
    map->setPosition(Vec2(0, 0));
    this->addChild(map);

    // Tạo player
    auto player1 = GameObjectManager::createPlayer();
    player1->setPosition(Vec2(660, 110));
    this->addChild(player1, -1);

    // Tạo camera cho player
    auto cameraPlayer = CameraController::create(player1);
    this->addChild(cameraPlayer);

    // Tạo collision chung
    CollisionManager::init();

    // Tạo UI
    auto uiLayer = Node::create(); // Tạo 1 node chứa UI
    this->addChild(uiLayer, 100); // Thêm uiLayer vào scene với z-index = 100 để đảm bảo luôn ở trên các đối tượng khác   
    // Tạo nút bấm
    auto buttons = UIManager::createMobileButtons();
    uiLayer->addChild(buttons); // Thêm vào node các UI cần
    // Tạo máu nhân vật
    auto heartLives = UIManager::createHeartLives();
    uiLayer->addChild(heartLives);
    // Tạo điểm số
    auto scorePlayer = UIManager::createScoreLabel();
    uiLayer->addChild(scorePlayer);
    // Tạo bộ đếm thời gian
    auto timer = UIManager::createGameTimer();
    uiLayer->addChild(timer);
    // Tạo setting in game
    auto settingINGame = UIManager::createSettingBoardInGame();
    uiLayer->addChild(settingINGame);

    // Tạo camera cho UI
    auto uiCamera = Camera::createOrthographic(640, 360, 1, 1000); // Tạo 1 camera trực giao
    uiCamera->setCameraFlag(CameraFlag::USER1); // Gán cờ riêng cho camera này tách UI ra khỏi camera mặc định của game
    uiCamera->setDepth(1); // Thêm độ sâu để sắp xếp thứ tự hiển thị những đối tượng của camera này, default = 0
    this->addChild(uiCamera);
    uiCamera->setPosition3D(this->getDefaultCamera()->getPosition3D()); // Đặt vị trí cameraUI mới
    this->getDefaultCamera()->setVisible(false);  // Ẩn camera mặc định  
    uiLayer->setCameraMask((int)(CameraFlag::USER1), true); // Gán node này vào camera UI
    

    // Tạo backgrond in game
    auto backgroundingame = UIManager::createBGInGame();
    this->addChild(backgroundingame, -100);
    // Tạo camera cho BG
    auto bgCamera = Camera::createOrthographic(640, 360, 1, 1000); // Tạo 1 camera trực giao
    bgCamera->setCameraFlag(CameraFlag::USER2); // Gán cờ riêng cho camera này tách UI ra khỏi camera mặc định của game
    bgCamera->setDepth(-2); // Thêm độ sâu để sắp xếp thứ tự hiển thị những đối tượng của camera này, default = 0
    this->addChild(bgCamera);
    bgCamera->setPosition3D(this->getDefaultCamera()->getPosition3D()); // Đặt vị trí cameraUI mới
    backgroundingame->setCameraMask((int)(CameraFlag::USER2), true);

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
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
            // Gọi GameManager ở update để chạy update của game manager
            GameManager::getInstance().update(delta);
            break;
        }
    }
}

Level_1_Scene::Level_1_Scene()
{
    AXLOGD("Scene: Level_1 ctor");
}

Level_1_Scene::~Level_1_Scene()
{
    AXLOGD("~Scene: Level_1 dtor");
}