#include "Trampoline.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"

USING_NS_AX;

Trampoline::Trampoline() : Trap()
{
    damage    = 0;
    isActive  = false;
    isDynamic = false;

    AXLOG("Bẫy (Trampoline) tạo thành công");
}
Trampoline::~Trampoline()
{
    AXLOG("Bẫy (Trampoline) bị huỷ");
    AX_SAFE_RELEASE(actTrap); // Xóa hành động vì đã lưu vào bộ nhớ
}

bool Trampoline::init()
{
    if (!Sprite::initWithTexture(SpriteManager::getInstance().getTextureByName("trampolineTrap")))
        return false;

    // Đặt AnchorPoint tại chính giữa, dưới chân (nếu Trampoline hướng lên trên)
    this->setAnchorPoint(Vec2(0.5, 0));

    // Tạo PhysicsBody mới
    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(ObjectBitmask::Trampoline);
        trapBody->setCollisionBitmask(ObjectBitmask::Player|ObjectBitmask::Ground);
        trapBody->setContactTestBitmask(ObjectBitmask::Player);

        // Gán PhysicsBody vào Sprite
        this->setPhysicsBody(trapBody);
    }
    this->setTag(static_cast<int>(TrapType::Trampoline));


    // Load sprite sheet
    auto frames = SpriteManager::getInstance().createVectorSpriteFrame("Trampoline_Jump_%d.png", 8);

    // Tạo Animation
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate   = Animate::create(animation);

    // Chạy Animation
    actTrap =  Repeat::create(animate, 1);
    actTrap->retain();
    return true;
}

void Trampoline::activateTrap()
{
    if (isActive)
        return;
    isActive = true;
    AXLOG("Bãy (Trampoline) đã kích hoạt!");

    // Chạy Animation
    this->runAction(actTrap);

    // Hẹn thời gian để tắt bẫy sau 0.8 giây
    this->scheduleOnce([this](float) { this->deactivateTrap(); }, 0.8f, "deactivate_trap");
}

void Trampoline::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (Trampoline) đã dừng!");
    // Dừng tất cả animation đang chạy
    this->stopAllActions();
}



