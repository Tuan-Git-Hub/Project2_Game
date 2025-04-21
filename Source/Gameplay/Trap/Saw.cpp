#include "Saw.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"

USING_NS_AX;

Saw::Saw() : Trap()
{
    damage    = 1;
    isActive  = true;
    isDynamic = false;
    amplitude = 0.0f;
    frequency = 0.0f;
    AXLOG("Bẫy (Saw) tạo thành công");
}
Saw::~Saw()
{
    AXLOG("Bẫy (Saw) bị huỷ");
}

bool Saw::init()
{
    if (!Sprite::initWithTexture(SpriteManager::getInstance().getTextureByName("sawTrap")))
        return false;

    // Tạo PhysicsBody mới
    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(ObjectBitmask::SawTrap);
        trapBody->setCollisionBitmask(ObjectBitmask::Player|ObjectBitmask::Ground);
        trapBody->setContactTestBitmask(ObjectBitmask::Player);

        // Gán PhysicsBody vào Sprite
        this->setPhysicsBody(trapBody);
    }
    this->setTag(static_cast<int>(TrapType::Saw));
    
    return true;
}

void Saw::activateTrap()
{
    if (isActive == false)
        isActive = true;

    AXLOG("Bãy (Saw) đã kích hoạt!");

    runAnimation();      // Gọi phương thức chạy hoạt ảnh
    startOscillation();  // Gọi phương thức dao động điều hòa
}

void Saw::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (Saw) đã dừng!");
}

void Saw::runAnimation()
{
    // Tạo hoạt ảnh
    auto frames = SpriteManager::getInstance().createVectorSpriteFrame("Saw_On_%d.png", 8);

    float period = 1.0f / frequency;

    // Tạo Animation
    auto animation = Animation::createWithSpriteFrames(frames, (period/100.0f));
    auto animate   = Animate::create(animation);

    // Chạy Animation lặp vô hạn
    this->runAction(RepeatForever::create(animate));
}

void Saw::startOscillation()
{
    AXLOG("Saw bắt đầu dao động điều hòa!");

    // Tạo hành động dao động

    // Tính chu kỳ
    float period = 1.0f / frequency;

    auto moveRight = MoveBy::create((period / 4.0f), Vec2(amplitude, 0));  // Di chuyển sang phải
    auto moveLeft  = MoveBy::create((period / 4.0f), Vec2(-amplitude, 0));  // Di chuyển sang trái

    // Tạo hành động tuần hoàn
    auto oscillate = RepeatForever::create(Sequence::create(moveRight, moveLeft, nullptr));

    // Chạy hành động
    this->runAction(oscillate);
}

void Saw::stopOscillation()
{
    AXLOG("Saw dừng dao động điều hòa!");
    this->stopAllActions();  // Dừng tất cả hành động (bao gồm cả dao động)
}

void Saw::setAmplitude(float _amp)
{
    this->amplitude=_amp;
}

float Saw::getAmplitude()
{
    return amplitude;
}

void Saw::setFrequency(float _freq)
{
    this->frequency = _freq;
}

float Saw::getFrequency()
{
    return frequency;
}
