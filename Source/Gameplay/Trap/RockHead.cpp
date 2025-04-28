#include "RockHead.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"

USING_NS_AX;

RockHead::RockHead() : Trap()
{
    damage    = 0;
    isActive  = true;
    isDynamic = false;
    amplitude = 0.0f;
    frequency = 0.0f;
    AXLOG("Bẫy (RockHead) tạo thành công");
}
RockHead::~RockHead()
{
    AXLOG("Bẫy (RockHead) bị huỷ");
}

bool RockHead::init()
{
    if (!Sprite::initWithTexture(SpriteManager::getInstance().getTextureByName("rockHead")))
        return false;

    // Tạo PhysicsBody mới
    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(ObjectBitmask::RockHead);
        trapBody->setCollisionBitmask(ObjectBitmask::Player|ObjectBitmask::Ground);
        trapBody->setContactTestBitmask(ObjectBitmask::Player);

        // Gán PhysicsBody vào Sprite
        this->setPhysicsBody(trapBody);
    }
    this->setTag(static_cast<int>(TrapType::RockHead));
    
    return true;
}

void RockHead::activateTrap(std::string axis)
{
    if (isActive == false)
        isActive = true;

    AXLOG("Bãy (RockHead) đã kích hoạt!");

    startBehavior(axis);  // Gọi phương thức thực hiện hành vi
}

void RockHead::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (RockHead) đã dừng!");
}

ax::Animation* RockHead::getAnimation(std::string animName)
{
    // Tạo các vectorFrames từ các frame
    // 1. blink
    auto blinkFrames = SpriteManager::getInstance().createVectorSpriteFrame("RockHead_Blink_%d.png", 4);
    
    // 2. leftHit
    auto  leftHitFrames = SpriteManager::getInstance().createVectorSpriteFrame("RockHead_LeftHit_%d.png", 4);
    leftHitFrames.pushBack(blinkFrames.back());
    
    // 3. rightHit
    auto rightHitFrames = SpriteManager::getInstance().createVectorSpriteFrame("RockHead_RightHit_%d.png", 4);
    rightHitFrames.pushBack(blinkFrames.back());
   
    // 4. topHit
    auto topHitFrames = SpriteManager::getInstance().createVectorSpriteFrame("RockHead_TopHit_%d.png", 4);
    topHitFrames.pushBack(blinkFrames.back());
    
    // 5. bottomHit
    auto bottomHitFrames = SpriteManager::getInstance().createVectorSpriteFrame("RockHead_BottomHit_%d.png", 4);
    bottomHitFrames.pushBack(blinkFrames.back());

    // Tạo Animation
    auto blinkAnim = Animation::createWithSpriteFrames(blinkFrames, 1.0f/20);
    auto leftHitAnim = Animation::createWithSpriteFrames(leftHitFrames, 1.0f/20);
    auto rightHitAnim = Animation::createWithSpriteFrames(rightHitFrames, 1.0f/20);
    auto topHitAnim   = Animation::createWithSpriteFrames(topHitFrames, 1.0f/20);
    auto bottomHitAnim = Animation::createWithSpriteFrames(bottomHitFrames, 1.0f/20);

    if (animName == "blinkAnim")
        return blinkAnim;
    else if (animName == "leftHitAnim")
        return leftHitAnim;
    else if (animName == "rightHitAnim")
        return rightHitAnim;
    else if (animName == "topHitAnim")
        return topHitAnim;
    else if (animName == "bottomHitAnim")
        return bottomHitAnim;
    else
    {
        AXLOG("Vui lòng truyền vào đúng cú pháp: 'blinkAnim', 'leftHitAnim', 'rightHitAnim', 'topHitAnim', 'bottomHitAnim'");
        return nullptr;
    }
}

void RockHead::startBehavior(std::string axis)
{
    AXLOG("RockHead bắt đầu hành vi của mình!");

    // Lấy hoạt ảnh:
    auto blinkAnim = this->getAnimation("blinkAnim");
    auto leftHitAnim = this->getAnimation("leftHitAnim");
    auto rightHitAnim = this->getAnimation("rightHitAnim");
    auto topHitAnim   = this->getAnimation("topHitAnim");
    auto bottomHitAnim = this->getAnimation("bottomHitAnim");


    // Tính chu kỳ
    float period = 1.0f / this->frequency;

    auto moveLeft  = MoveBy::create(period/4, Vec2(-amplitude, 0));
    auto moveRight = MoveBy::create(period/4, Vec2(+amplitude, 0));
    auto moveUp    = MoveBy::create(period / 4, Vec2(0, +amplitude));
    auto moveDown = MoveBy::create(period / 4, Vec2(0, -amplitude));

    auto blink    = Animate::create(blinkAnim);

    auto leftHit  = Animate::create(leftHitAnim);
    auto rightHit = Animate::create(rightHitAnim);
    auto topHit   = Animate::create(topHitAnim);
    auto bottomHit = Animate::create(bottomHitAnim);

    auto wait = DelayTime::create(0.2f);  // dừng 1 giây

    auto moveLeftWithBlink  = Spawn::createWithTwoActions(moveLeft, blink);
    auto moveRightWithBlink = Spawn::createWithTwoActions(moveRight, blink);
    auto moveUpWithBlink    = Spawn::createWithTwoActions(moveUp, blink);
    auto moveDownWithBlink  = Spawn::createWithTwoActions(moveDown, blink);

    auto horizontalSeq =
        Sequence::create(moveLeftWithBlink, leftHit, wait, moveRightWithBlink, rightHit, wait, nullptr);
    auto verticalSeq =
        Sequence::create(moveUpWithBlink, topHit, wait, moveDownWithBlink, bottomHit, wait, nullptr);

    // Lặp lại mãi mãi
    if (axis == "horizontal")
        this->runAction(RepeatForever::create(horizontalSeq));
    else if (axis == "vertical")
        this->runAction(RepeatForever::create(verticalSeq));
    else
    {
        AXLOG("Vui lòng truyền vào đúng cú pháp: 'horizontal', hoặc 'vertical'");
        return;
    }
}

void RockHead::stopBehavior()
{
    AXLOG("RockHead bắt đầu hành vi của mình!");
    this->stopAllActions();  // Dừng tất cả hành động (bao gồm cả dao động)
}

void RockHead::setAmplitude(float _amp)
{
    this->amplitude=_amp;
}

float RockHead::getAmplitude()
{
    return amplitude;
}

void RockHead::setFrequency(float _freq)
{
    this->frequency = _freq;
}

float RockHead::getFrequency()
{
    return frequency;
}
