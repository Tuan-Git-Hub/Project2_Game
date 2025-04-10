#include "SensorScene.h"

using namespace ax;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "SensorScene.cpp\n");
}

Scene* SensorScene::createScene()
{
    auto scene = Scene::createWithPhysics();  // Scene có Physics
    auto layer = SensorScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool SensorScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    this->getPhysicsWorld()->setAutoStep(true);

    // set gravity
    this->getPhysicsWorld()->setGravity(Vec2(0, -980));  // 9.8 m/s^2

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                           AX_CALLBACK_1(SensorScene::menuCloseCallback, this));

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
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

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto s_centre = Vec2(visibleSize.width / 2, visibleSize.height / 2);
    auto bodyWall = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    bodyWall->setCategoryBitmask(0x03);
    bodyWall->setCollisionBitmask(0x01 || 0x02);
    bodyWall->setContactTestBitmask(0x01 || 0x02);
    auto edgeNode = Node::create();
    edgeNode->setPosition(s_centre);
    edgeNode->setPhysicsBody(bodyWall);
    this->addChild(edgeNode);

    /////////////////////////////
    // 3. add your codes below...

    // Some templates (uncomment what you  need)
    auto touchListener            = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = AX_CALLBACK_2(SensorScene::onTouchesBegan, this);
    touchListener->onTouchesMoved = AX_CALLBACK_2(SensorScene::onTouchesMoved, this);
    touchListener->onTouchesEnded = AX_CALLBACK_2(SensorScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(
            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().y));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // 🌟 Tạo vùng Sensor (hình chữ nhật)
    auto sensorNode = Node::create();
    sensorNode->setPosition(Vec2(visibleSize.width / 2, 0));

    auto sensorBody = PhysicsBody::createBox(Size(24, 100), PhysicsMaterial(0.1f, 0.0f, 0.0f));
    sensorBody->setDynamic(false);            // Không bị ảnh hưởng bởi trọng lực
    sensorBody->setCategoryBitmask(0x04);     // Loại vật thể là Sensor
    sensorBody->setCollisionBitmask(0x00);    // Không va chạm với bất kỳ vật thể nào
    sensorBody->setContactTestBitmask(0x01);  // Phát hiện vật thể khác đi qua
    sensorBody->setTag(99);                   // Đánh dấu là Sensor

    sensorNode->setPhysicsBody(sensorBody);
    this->addChild(sensorNode);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png"sv);
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        sprite->setScale(0.20f);
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // create a physics body for the sprite
        auto bodyLogo = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));
        bodyLogo->setDynamic(true);
        bodyLogo->setGravityEnable(true);
        bodyLogo->setCategoryBitmask(0x01);       // Loại vật thể là Player
        bodyLogo->setCollisionBitmask(0xFF);      // Có thể va chạm với mọi thứ
        bodyLogo->setContactTestBitmask(0x04);    // Có thể kích hoạt Sensor
        sprite->setPhysicsBody(bodyLogo);

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
        auto drawNode = DrawNode::create();
        drawNode->setPosition(Vec2(0, 0));
        addChild(drawNode);

        drawNode->drawRect(safeArea.origin + Vec2(1, 1), safeArea.origin + safeArea.size, Color4B::BLUE);
    }

    // 🌟 Bắt sự kiện va chạm
    auto contactListener               = EventListenerPhysicsContact::create();
    contactListener->onContactBegin    = AX_CALLBACK_1(SensorScene::onContactBegin, this);
    contactListener->onContactSeparate = AX_CALLBACK_1(SensorScene::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    auto keyboardListener           = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed  = AX_CALLBACK_2(SensorScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = AX_CALLBACK_2(SensorScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, sprite);

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

// 🌟 Danh sách các vật thể hiện đang ở trong Sensor
std::unordered_set<ax::Node*> objectsInSensor;

bool SensorScene::onContactBegin(PhysicsContact& contact)
{
    auto shapeA = contact.getShapeA();
    auto shapeB = contact.getShapeB();

    auto bodyA = shapeA->getBody();
    auto bodyB = shapeB->getBody();

    // Kiểm tra nếu một trong hai là Sensor
    if (bodyA->getCategoryBitmask() == 0x04)
    {
        AXLOG("before IN objectInSensor");
        objectsInSensor.insert(bodyB->getNode());
        AXLOG("📢 Vật thể vào vùng Sensor!");
    }
    else if (bodyB->getCategoryBitmask() == 0x04)
    {
        AXLOG("before IN objectInSensor");
        objectsInSensor.insert(bodyA->getNode());
        AXLOG("📢 Vật thể vào vùng Sensor!");
    }

    return true;
}

bool SensorScene::onContactSeparate(PhysicsContact& contact)
{
    auto shapeA = contact.getShapeA();
    auto shapeB = contact.getShapeB();

    auto bodyA = shapeA->getBody();
    auto bodyB = shapeB->getBody();

    // Kiểm tra nếu một trong hai là Sensor
    if (bodyA->getCategoryBitmask() == 0x04)
    {
        AXLOG("before OUT objectInSensor");
        objectsInSensor.erase(bodyB->getNode());
        AXLOG("🚪 Vật thể rời khỏi vùng Sensor!");
    }
    else if (bodyB->getCategoryBitmask() == 0x04)
    {
        AXLOG("before OUT objectInSensor");
        objectsInSensor.erase(bodyA->getNode());
        AXLOG("🚪 Vật thể rời khỏi vùng Sensor!");
    }

    return true;
}

void SensorScene::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesBegan detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void SensorScene::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesMoved detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void SensorScene::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesEnded detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}


void SensorScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    AXLOG("onKeyPressed, keycode: %d", static_cast<int>(code));

    auto node = event->getCurrentTarget();
    if (!node)
        return;

    auto sprite = dynamic_cast<Sprite*>(node);
    if (!sprite)
        return;

    auto body = sprite->getPhysicsBody();
    if (!body)
        return;

    static std::unordered_map<EventKeyboard::KeyCode, Vec2> keyMovements = {
        {EventKeyboard::KeyCode::KEY_RIGHT_ARROW, Vec2(100, 0)},  // Sang phải
        {EventKeyboard::KeyCode::KEY_LEFT_ARROW, Vec2(-100, 0)},  // Sang trái
        {EventKeyboard::KeyCode::KEY_UP_ARROW, Vec2(0, 200)},     // Nhảy cao
    };

    // Kiểm tra nếu phím được ánh xạ
    auto it = keyMovements.find(code);
    if (it != keyMovements.end())
    {
        Vec2 velocity = body->getVelocity();

        // Kiểm tra nếu nhấn hai phím cùng lúc (nhảy chéo)
        if (code == EventKeyboard::KeyCode::KEY_UP_ARROW)
        {
            // Nếu đang chạy -> tăng khoảng cách nhảy
            if (std::abs(velocity.x) > 10)
            {
                velocity.y = 250;  // Nhảy xa hơn khi chạy lấy đà
            }
            else
            {
                velocity.y = 200;  // Nhảy bình thường
            }
        }
        else
        {
            velocity.x = it->second.x;  // Cập nhật vận tốc ngang
        }

        body->setVelocity(velocity);  // Áp dụng vận tốc
    }
}

void SensorScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
    AXLOG("onKeyReleased, keycode: %d", static_cast<int>(code));
}

void SensorScene::update(float delta)
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

void SensorScene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
