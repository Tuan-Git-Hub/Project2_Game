#include "MobileButtons.h"
#include "SpriteManager.h"

using namespace ax;

MobileButtons* MobileButtons::instance = nullptr;

MobileButtons* MobileButtons::getInstance()
{
    if (!instance)
    {
        instance = new MobileButtons();
        if (instance && instance->init())
        {
            instance->retain(); // giữ lại giúp không tự bị xóa khi hết scene
            return instance;
        }
        delete instance;
        instance = nullptr;
        return nullptr;
    }
    return instance;
}

bool MobileButtons::init()
{
    if (!Node::init()) 
    {
        return false;
    }

    // Tạo nút di chuyển sang trái
    auto leftMoveButton = SpriteManager::getInstance().createSprite("leftButton");
    leftMoveButton->setPosition(100, 60);
    leftMoveButton->setScale(2.0f); 
    this->addChild(leftMoveButton);

    // Tạo nút di chuyển sang phải
    auto rightMoveButton = SpriteManager::getInstance().createSprite("rightButton");
    rightMoveButton->setPosition(180, 60);
    rightMoveButton->setScale(2.0f);
    this->addChild(rightMoveButton);

    // Tạo nút di chuyển nhảy
    auto jumpMoveButton = SpriteManager::getInstance().createSprite("jumpButton");
    jumpMoveButton->setPosition(500, 60);
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

    _eventDispatcher->addEventListenerWithSceneGraphPriority(leftMoveListener ,leftMoveButton);

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

    _eventDispatcher->addEventListenerWithSceneGraphPriority(rightMoveListener ,rightMoveButton);


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
                isHoldingJump = true;
                jumpMove();
                return true;
            }
        }
        return false;
    };

    jumpMoveListener->onTouchEnded = [this, jumpMoveButton](Touch* touch, Event* event){
        jumpMoveButton->setColor(Color3B::WHITE); // Trả lại màu như cũ
        isHoldingJump = false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(jumpMoveListener ,jumpMoveButton);

    scheduleUpdate(); // Cập nhật mỗi frame

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

void MobileButtons::deleteInstance()
{
    if (instance)
    {
        instance->release();
        instance = nullptr;
    }
}