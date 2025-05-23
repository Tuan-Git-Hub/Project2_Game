#include "MobileButtons.h"
#include "SpriteManager.h"

using namespace ax;

std::function<void()> MobileButtons::leftMove;
std::function<void()> MobileButtons::rightMove;
std::function<void()> MobileButtons::jumpMove;
std::function<void()> MobileButtons::stopMove;

MobileButtons* MobileButtons::createMobileButtons()
{

    MobileButtons* ret = new MobileButtons();
    if (ret && ret->init())
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool MobileButtons::init()
{
    if (!Node::init()) 
    {
        return false;
    }
    // Đặt tên
    this->setName("Mobile_Buttons");

    // Tạo nút di chuyển sang trái
    auto leftMoveButton = SpriteManager::getInstance().createSprite("leftButton");
    leftMoveButton->setPosition(60, 60);
    leftMoveButton->setScale(2.0f); 
    this->addChild(leftMoveButton);

    // Tạo nút di chuyển sang phải
    auto rightMoveButton = SpriteManager::getInstance().createSprite("rightButton");
    rightMoveButton->setPosition(140, 60);
    rightMoveButton->setScale(2.0f);
    this->addChild(rightMoveButton);

    // Tạo nút di chuyển nhảy
    auto jumpMoveButton = SpriteManager::getInstance().createSprite("jumpButton");
    jumpMoveButton->setPosition(540, 60);
    jumpMoveButton->setScale(2.0f);
    this->addChild(jumpMoveButton);

    // Tạo EventListenerTouchOneByOne cho nút di chuyển sang trái
    auto leftMoveListener = EventListenerTouchOneByOne::create();
    leftMoveListener->setSwallowTouches(true);
    leftMoveListener->onTouchBegan = [this, leftMoveButton](Touch* touch, Event* event){
        if (leftMoveButton->getBoundingBox().containsPoint(touch->getLocation()))
        {
            if (leftMove)
            {
                leftMoveButton->setColor(Color3B(150, 150, 150)); // Làm tối hơn;
                AXLOG("Main character moved to the left!");
                isHoldingMoveLeft = true;
                leftMove();
                return true;
            }
        }
        return false;
    };

    leftMoveListener->onTouchEnded = [this, leftMoveButton](Touch* touch, Event* event){
        if(stopMove)
        {   
            leftMoveButton->setColor(Color3B::WHITE); // Trả lại màu như cũ
            isHoldingMoveLeft = false;
            stopMove();
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(leftMoveListener ,this);

    // Tạo EventListenerTouchOneByOne cho nút di chuyển sang phải
    auto rightMoveListener = EventListenerTouchOneByOne::create();
    rightMoveListener->setSwallowTouches(true);
    rightMoveListener->onTouchBegan = [this, rightMoveButton](Touch* touch, Event* event){
        if (rightMoveButton->getBoundingBox().containsPoint(touch->getLocation()))
        {
            if (rightMove)
            {
                rightMoveButton->setColor(Color3B(150, 150, 150)); // Làm tối hơn;
                AXLOG("Main character moved to the right!");
                isHoldingMoveRight = true;
                rightMove();
                return true;
            }
        }
        return false;
    };

    rightMoveListener->onTouchEnded = [this, rightMoveButton](Touch* touch, Event* event){
        if(stopMove)
        {   
            rightMoveButton->setColor(Color3B::WHITE); // Trả lại màu như cũ
            isHoldingMoveRight = false;
            stopMove();
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(rightMoveListener ,this);


    // Tạo EventListenerTouchOneByOne cho nút nhảy
    auto jumpMoveListener = EventListenerTouchOneByOne::create();
    jumpMoveListener->setSwallowTouches(true);
    jumpMoveListener->onTouchBegan = [this, jumpMoveButton](Touch* touch, Event* event){
        if (jumpMoveButton->getBoundingBox().containsPoint(touch->getLocation()))
        {
            if (rightMove)
            {
                jumpMoveButton->setColor(Color3B(150, 150, 150)); // Làm tối hơn;
                AXLOG("Main character jumped!");
                jumpMove();
                return true;
            }
        }
        return false;
    };

    jumpMoveListener->onTouchEnded = [this, jumpMoveButton](Touch* touch, Event* event){
        jumpMoveButton->setColor(Color3B::WHITE); // Trả lại màu như cũ
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(jumpMoveListener ,this);

    // Tạo sự kiện nhấn nút
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode code, Event* event) {
        if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            isHoldingMoveRight = true;
        }
        else if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            isHoldingMoveLeft = true;
        }
        else if (code == EventKeyboard::KeyCode::KEY_UP_ARROW)
        {
            jumpMove();
        }
    };
    keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode code, Event* event) {
        if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW || code == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            isHoldingMoveRight = false;
            isHoldingMoveLeft = false;
            stopMove();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
    scheduleUpdateWithPriority(1); // Cập nhật mỗi frame

    return true;
}

void MobileButtons::update(float dt)
{
    if (isHoldingMoveLeft)
    {
        leftMove();
    }
    else if (isHoldingMoveRight)
    {
        rightMove();
    }
}