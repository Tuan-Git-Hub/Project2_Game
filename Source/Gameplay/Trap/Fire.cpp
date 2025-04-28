#include "Fire.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"
#include "SoundManager.h"

USING_NS_AX;

Fire::Fire() : Trap()
{
    damage    = 1;
    isActive  = false;
    isDynamic = false;

    AXLOG("Bẫy (Fire) tạo thành công");
}
Fire::~Fire()
{
    AXLOG("Bẫy (Fire) bị huỷ");
    AX_SAFE_RELEASE(actTrap); // Xóa hành động vì đã lưu vào bộ nhớ
}

bool Fire::init()
{
    if (!Sprite::initWithTexture(SpriteManager::getInstance().getTextureByName("fireTrap"))) 
        return false;

    // Đặt AnchorPoint tại chính giữa, dưới chân (nếu Fire hướng lên trên)
    this->setAnchorPoint(Vec2(0.5, 0));

    // Tạo PhysicsBody mới
    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(ObjectBitmask::FireTrap);
        trapBody->setCollisionBitmask(ObjectBitmask::Player|ObjectBitmask::Ground);
        trapBody->setContactTestBitmask(ObjectBitmask::Player);

        // Gán PhysicsBody vào Sprite
        this->setPhysicsBody(trapBody);
    }
    this->setTag(static_cast<int>(TrapType::Fire));

    // Load sprite sheet
    auto frames = SpriteManager::getInstance().createVectorSpriteFrame("Fire_On_%d.png", 3);

    // Tạo Animation
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate   = Animate::create(animation);

    // Chạy Animation
    actTrap =  RepeatForever::create(animate);
    actTrap->retain();

    return true;
}

void Fire::activateTrap()
{
    if (isActive)
        return;

    AXLOG("Bãy (Fire) đã kích hoạt!");

    // Chạy Animation
    this->stopAllActions();
    this->runAction(Sequence::create(
        DelayTime::create(1.0f),
        CallFunc::create([this](){
            SoundManager::playEffect(AudioPaths::FIRE);
            isActive = true;
            this->runAction(actTrap);
        }),
        nullptr
    ));

    // Hẹn thời gian để tắt bẫy sau 0.3 giây
    this->scheduleOnce([this](float) { this->deactivateTrap(); }, 3.0f, "deactivate_trap");
}

void Fire::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (Fire) đã dừng!");
    // Dừng tất cả animation đang chạy
    this->stopAllActions();

    auto spriteFire = SpriteManager::getInstance().getTextureByName("fireTrap");
    this->setTexture(spriteFire);
    this->setTextureRect(Rect(0, 0, spriteFire->getContentSize().width, spriteFire->getContentSize().height));
}



