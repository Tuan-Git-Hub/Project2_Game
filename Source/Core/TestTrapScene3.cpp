#include "TestTrapScene3.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "CameraController.h"
#include "CollisionManager.h"
#include "UIManager.h"

using namespace ax;

bool TestTrapScene3::init()
{
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    this->getPhysicsWorld()->setAutoStep(true);

    // set gravity
    this->getPhysicsWorld()->setGravity(Vec2(0, -900)); // 9.8 m/s^2
    // Tạo collision chung
    CollisionManager::init();

    // Chọn level để game manager tạo logic game
    GameManager::getInstance().selectLevel(GameManager::Level::LEVEL_3);

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    auto safeArea = _director->getSafeAreaRect();
    auto safeOrigin = safeArea.origin;

    // Tạo 1 wall bao quanh để test
    auto s_centre = Vec2(visibleSize.width / 2, visibleSize.height / 2);
    auto bodyWall = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    bodyWall->setCategoryBitmask(ObjectBitmask::Ground);
    bodyWall->setCollisionBitmask(0xFFFFFFFF);
    bodyWall->setContactTestBitmask(ObjectBitmask::Player);
    auto edgeNode = Node::create();
    edgeNode->setPosition(s_centre);
    edgeNode->setPhysicsBody(bodyWall);
    edgeNode->setName("WallTest");
    this->addChild(edgeNode);

    // player
    auto player1 = GameObjectManager::createPlayer();
    player1->setPosition(Vec2(100, 100));
    this->addChild(player1);

    // Tạo camera cho player
    auto cameraPlayer = CameraController::create(player1);
    this->addChild(cameraPlayer);

    // Tạo UI
    auto uiLayer = Node::create(); // Tạo 1 node chứa UI
    this->addChild(uiLayer, 100); // Thêm uiLayer vào scene với z-index = 100 để đảm bảo luôn ở trên các đối tượng khác   
    // Tạo nút bấm
    auto buttons = UIManager::createMobileButtons();
    buttons->setName("Mobile_Buttons");
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

    // Spike Trap
    auto spikeTrap1 = GameObjectManager::createTrap(TrapType::Spike);
    spikeTrap1->setPosition(Vec2(visibleSize.width * 1/4 , spikeTrap1->getContentSize().height/2));
    this->addChild(spikeTrap1);

    // Saw Trap
    // auto sawTrap = static_cast<Saw*>(GameObjectManager::createTrap(TrapType::Saw));
    // sawTrap->setAmplitude(200.0f);
    // sawTrap->setFrequency(0.25f);
    // sawTrap->setPosition(Vec2(visibleSize.width * 1/4 , visibleSize.height * 1/4));
    // sawTrap->activateTrap();
    // this->addChild(sawTrap);

    // Trampoline
    // auto trampolineTrap1 = GameObjectManager::createTrap(TrapType::Trampoline);
    // trampolineTrap1->setPosition(Vec2(visibleSize.width * 2/4 , 0));
    // this->addChild(trampolineTrap1);
    // auto trampolineTrap2 = GameObjectManager::createTrap(TrapType::Trampoline);
    // trampolineTrap2->setRotation(-90.0f);
    // trampolineTrap2->setPosition(Vec2(visibleSize.width, 20));
    // this->addChild(trampolineTrap2);

    // Fire trap
    // auto fireTrap1 = GameObjectManager::createTrap(TrapType::Fire);
    // fireTrap1->setPosition(Vec2(visibleSize.width * 3/4 , 0));
    // this->addChild(fireTrap1);
    // auto fireTrap2 = GameObjectManager::createTrap(TrapType::Fire);
    // fireTrap2->setPosition(Vec2(visibleSize.width, 50));
    // fireTrap2->setRotation(-90);
    // this->addChild(fireTrap2);

    // Rock Head
    auto rockHead = static_cast<RockHead*>(GameObjectManager::createTrap(TrapType::RockHead));
    rockHead->setPosition(Vec2(visibleSize.width * 3/4 , rockHead->getContentSize().height/2));
    rockHead->setAmplitude(200.0f);
    rockHead->setFrequency(0.1f);
    rockHead->activateTrap("horizontal");
    this->addChild(rockHead);

    // Fan
    // auto fan = GameObjectManager::createTrap(TrapType::Fan);
    // fan->setPosition(Vec2(visibleSize.width * 3/4 , 0));
    // this->addChild(fan);
    // auto fan2 = GameObjectManager::createTrap(TrapType::Fan);
    // fan2->setPosition(Vec2(visibleSize.width, 15));
    // fan2->setRotation(-90);
    // this->addChild(fan2);

    // Box
    // auto box1 = GameObjectManager::createItem(ItemType::Box1);
    // box1->setPosition(Vec2(visibleSize.width * 1/6 , visibleSize.height * 1/5));
    // this->addChild(box1);

    // auto box1_1 = GameObjectManager::createItem(ItemType::Box1);
    // box1_1->setPosition(Vec2(visibleSize.width * 2/4 + 40, visibleSize.height * 1/5));
    // this->addChild(box1_1);

    // auto box2 = GameObjectManager::createItem(ItemType::Box2);
    // box2->setPosition(Vec2(visibleSize.width * 2/4 + 80, visibleSize.height * 1/5));
    // this->addChild(box2);

    // auto box3 = GameObjectManager::createItem(ItemType::Box3);
    // box3->setPosition(Vec2(visibleSize.width * 2/4 + 120, visibleSize.height * 1/5));
    // this->addChild(box3);

    // Fruits
    auto fruit = GameObjectManager::createItem(ItemType::Fruits);
    fruit->setPosition(Vec2(visibleSize.width * 2/4 + 160, visibleSize.height * 1/5));
    this->addChild(fruit);


    scheduleUpdate();

    return true;
}

void TestTrapScene3::update(float delta)
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
        /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateJoyStick();
        // UpdatePlayer();
        // UpdatePhysics();
        // ...
        break;
    }

    case GameState::end:
    {  /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        _director->end();
        break;
    }

    }  // switch
}

TestTrapScene3::TestTrapScene3()
{
    AXLOG("Scene: TestTrap ctor");
}

TestTrapScene3::~TestTrapScene3()
{
    AXLOG("~Scene: TestTrap dtor");
}