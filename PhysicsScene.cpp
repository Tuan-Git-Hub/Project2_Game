/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "PhysicsScene.h"
#include <cmath>
#include "TrapFactory.h"
#include "CollisionHandler.h"

using namespace ax;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "PhysicsScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PhysicsScene::init()
{
    //////////////////////////////
    // 1. intialize the scene with physics world
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    this->getPhysicsWorld()->setAutoStep(true);

    // set gravity
    this->getPhysicsWorld()->setGravity(Vec2(0, -980)); // 9.8 m/s^2

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    auto safeArea = _director->getSafeAreaRect();
    auto safeOrigin = safeArea.origin;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        AX_CALLBACK_1(PhysicsScene::menuCloseCallback, this));

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

    auto s_centre    = Vec2(visibleSize.width / 2, visibleSize.height / 2);
    auto bodyWall        = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);
    bodyWall->setCategoryBitmask(0x03);
    bodyWall->setCollisionBitmask(0x01 || 0x02);
    bodyWall->setContactTestBitmask(0x01 || 0x02);
    auto edgeNode    = Node::create();
    edgeNode->setPosition(s_centre);
    edgeNode->setPhysicsBody(bodyWall);
    this->addChild(edgeNode);
    
    // add a label
    // create and initialize a label

    auto label = Label::createWithTTF("Physics Scene - Traps Demostation", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(
            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
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
        auto bodyLogo = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.015f, 1.0f, 0.0f));
        bodyLogo->setDynamic(true);
        bodyLogo->setGravityEnable(true);
        bodyLogo->setCategoryBitmask(0x01);
        bodyLogo->setCollisionBitmask(0x02|0x03);
        bodyLogo->setContactTestBitmask(0x02|0x03|0x04);
        sprite->setPhysicsBody(bodyLogo);

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
        auto drawNode = DrawNode::create();
        drawNode->setPosition(Vec2(0, 0));
        addChild(drawNode);

        drawNode->drawRect(safeArea.origin + Vec2(1, 1), safeArea.origin + safeArea.size, Color4B::BLUE);
    }

    // Spike Trap
    spikeTrap = TrapFactory::createTrap(TrapType::Spike, "Traps/Spikes/Spike_Idle.png");
    spikeTrap->setPosition(Vec2(visibleSize.width * 1/4 , spikeTrap->getContentSize().height/2));
    this->addChild(spikeTrap);

    // Trampoline Trap
    trampolineTrap = TrapFactory::createTrap(TrapType::Trampoline, "Traps/Trampoline/Trampoline_Idle.png");
    // Caution: AnchorPoint: (0.5, 0)
    trampolineTrap->setPosition(Vec2(visibleSize.width, trampolineTrap->getContentSize().width/2 + 30));
    // Thử quay 90 độ
    trampolineTrap->setRotation(-90.0f);
    this->addChild(trampolineTrap);

    // Fire Trap
    fireTrap = TrapFactory::createTrap(TrapType::Fire, "Traps/Fire/Fire_Off.png");
    fireTrap->setPosition(Vec2(visibleSize.width * 3 / 4, 0));
    this->addChild(fireTrap);

    // SMI Sand
    sand = TrapFactory::createTrap(TrapType::SandMudIce, "Traps/Sand Mud Ice/Sand.png", SMIType::Sand);
    sand->setPosition(
        Vec2(visibleSize.width * 2 / 4 - sand->getContentSize().width, sand->getContentSize().height / 2 ));
    this->addChild(sand);

    // SMI Mud
    mud = TrapFactory::createTrap(TrapType::SandMudIce, "Traps/Sand Mud Ice/Mud.png", SMIType::Mud);
    mud->setPosition(Vec2(visibleSize.width * 2 / 4, mud->getContentSize().height / 2 ));
    this->addChild(mud);

    // SMI Ice
    ice = TrapFactory::createTrap(TrapType::SandMudIce, "Traps/Sand Mud Ice/Ice.png", SMIType::Ice);
    ice->setPosition(
        Vec2(visibleSize.width * 2 / 4 + ice->getContentSize().width, ice->getContentSize().height / 2 ));
    this->addChild(ice);

    // Saw Trap
    sawTrap = dynamic_cast<Saw*>(TrapFactory::createTrap(TrapType::Saw, "Traps/Saw/Saw_Off.png"));
    sawTrap->setAmplitude(50.0f);
    sawTrap->setFrequency(1/0.8f);
    sawTrap->setPosition(
        Vec2(visibleSize.width * 1 / 3 , visibleSize.height * 2/3));
    sawTrap->activateTrap();
    this->addChild(sawTrap);

    // Fan Trap
    fanTrap1 = dynamic_cast<Fan*>(TrapFactory::createTrap(TrapType::Fan, "Traps/Fan/Fan_Off.png"));
    fanTrap1->setPosition(Vec2(visibleSize.width * 1 / 5, visibleSize.height / 8));
    fanTrap1->activateTrap();
    this->addChild(fanTrap1);

    fanTrap2 = dynamic_cast<Fan*>(TrapFactory::createTrap(TrapType::Fan, "Traps/Fan/Fan_Off.png"));
    fanTrap2->setPosition(Vec2(visibleSize.width * 1 / 5 - fanTrap2->getContentSize().width, visibleSize.height / 8));
    fanTrap2->activateTrap();
    this->addChild(fanTrap2);

    /*auto windZone = fanTrap->getWindZone();
    if (!windZone->getPhysicsBody()->isEnabled())
    {
        AXLOG("PhysicsBody bị tắt");
        return false;
    }

    if (windZone->getParent() == nullptr)
    {
        AXLOG("windZone không nẳm trong Scene Graph");
        return false;
    }
    else if (windZone->getParent() != fanTrap)
    {
        AXLOG("windZone không phải Node con của fanTrap");
        return false;
    }
    else
    {
        AXLOG("WindZone size: %.2f x %.2f", windZone->getContentSize().width, windZone->getContentSize().height);
        AXLOG("FanTrap pos: (%.1f, %.1f)", fanTrap->getPositionX(), fanTrap->getPositionY());
        AXLOG("WindZone local pos: (%.1f, %.1f)", windZone->getPositionX(), windZone->getPositionY());

        auto worldPos = windZone->getParent()->convertToWorldSpace(windZone->getPosition());
        AXLOG("WindZone global pos: (%.1f, %.1f)", worldPos.x, worldPos.y);

        int wzTag = windZone->getPhysicsBody()->getTag();
        int wzBitMask = windZone->getPhysicsBody()->getCategoryBitmask();
        AXLOG("WindZone tag: %d, WindZone bitmask: %d", wzTag, wzBitMask);
    }*/

    // Rock Head
    rockHead =
        dynamic_cast<RockHead*>(TrapFactory::createTrap(TrapType::RockHead, "Traps/Rock Head/RockHead_Idle.png"));
    rockHead->setAmplitude(100.0f);
    rockHead->setFrequency(1.0f);
    rockHead->setPosition(Vec2(visibleSize.width * 1 / 8, rockHead->getContentSize().height/2));
    rockHead->activateTrap("horizontal");
    this->addChild(rockHead);

    /////////////////////////////
    // 3. add your codes below...

    // Some templates (uncomment what you  need)
    auto touchListener            = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = AX_CALLBACK_2(PhysicsScene::onTouchesBegan, this);
    touchListener->onTouchesMoved = AX_CALLBACK_2(PhysicsScene::onTouchesMoved, this);
    touchListener->onTouchesEnded = AX_CALLBACK_2(PhysicsScene::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // auto mouseListener           = EventListenerMouse::create();
    // mouseListener->onMouseMove   = AX_CALLBACK_1(PhysicsScene::onMouseMove, this);
    // mouseListener->onMouseUp     = AX_CALLBACK_1(PhysicsScene::onMouseUp, this);
    // mouseListener->onMouseDown   = AX_CALLBACK_1(PhysicsScene::onMouseDown, this);
    // mouseListener->onMouseScroll = AX_CALLBACK_1(PhysicsScene::onMouseScroll, this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    auto keyboardListener           = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed  = AX_CALLBACK_2(PhysicsScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = AX_CALLBACK_2(PhysicsScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, sprite);

    // Tạo EventListener để bắt va chạm với vật thể cứng
    auto contactListener1 = ax::EventListenerPhysicsContact::create();

    // Xử lý khi va chạm xảy ra
    contactListener1->onContactBegin    = CollisionHandler::onContactBegin;
    contactListener1->onContactPreSolve = CollisionHandler::onContactPreSolve;

    // Gán listener vào Scene
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener1, this);

    // Tạo EventListener để bắt va chạm với sensorZone
    auto contactListener2 = ax::EventListenerPhysicsContact::create();

    // Xử lý khi va chạm xảy ra
    contactListener2->onContactBegin    = CollisionHandler::onSensorContactBegin;
    contactListener2->onContactSeparate = CollisionHandler::onSensorContactSeparate;

    // Gán listener vào Scene
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener2, this);

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}


void PhysicsScene::onTouchesBegan(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesBegan detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void PhysicsScene::onTouchesMoved(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesMoved detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

void PhysicsScene::onTouchesEnded(const std::vector<ax::Touch*>& touches, ax::Event* event)
{
    for (auto&& t : touches)
    {
        AXLOG("onTouchesEnded detected, X:%f  Y:%f", t->getLocation().x, t->getLocation().y);
    }
}

// void PhysicsScene::onMouseDown(Event* event)
// {
//     EventMouse* e = static_cast<EventMouse*>(event);
//     AXLOG("onMouseDown detected, Key: %d", static_cast<int>(e->getMouseButton()));
// }

// void PhysicsScene::onMouseUp(Event* event)
// {
//     EventMouse* e = static_cast<EventMouse*>(event);
//     AXLOG("onMouseUp detected, Key: %d", static_cast<int>(e->getMouseButton()));
// }

// void PhysicsScene::onMouseMove(Event* event)
// {
//     EventMouse* e = static_cast<EventMouse*>(event);
//     AXLOG("onMouseMove detected, X:%f  Y:%f", e->getCursorX(), e->getCursorY());
// }

// void PhysicsScene::onMouseScroll(Event* event)
// {
//     EventMouse* e = static_cast<EventMouse*>(event);
//     AXLOG("onMouseScroll detected, X:%f  Y:%f", e->getScrollX(), e->getScrollY());
// }

void PhysicsScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event)
{
    // AXLOG("onKeyPressed, keycode: %d", static_cast<int>(code));

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
        {EventKeyboard::KeyCode::KEY_RIGHT_ARROW, Vec2(150, 0)},  // Sang phải
        {EventKeyboard::KeyCode::KEY_LEFT_ARROW, Vec2(-150, 0)},  // Sang trái
        {EventKeyboard::KeyCode::KEY_UP_ARROW, Vec2(0, 450)},     // Nhảy cao
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
            if (std::abs(velocity.x) > 50)
            {
                velocity.y = 550;  // Nhảy xa hơn khi chạy lấy đà
            }
            else
            {
                velocity.y = 450;  // Nhảy bình thường
            }
        }
        else
        {
            velocity.x = it->second.x;  // Cập nhật vận tốc ngang
        }

        body->setVelocity(velocity);  // Áp dụng vận tốc
    }
}



void PhysicsScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event)
{
    // AXLOG("onKeyReleased, keycode: %d", static_cast<int>(code));
    return;
}


void PhysicsScene::update(float delta)
{
    
}

void PhysicsScene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

     // EventCustom customEndEvent("game_scene_close_event");
     //_eventDispatcher->dispatchEvent(&customEndEvent);
}
