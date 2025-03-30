#include "Spike.h"

USING_NS_AX;

Spike::Spike() : Trap()
{
    damage    = 1;
    isActive  = true;
    isDynamic = false;

    AXLOG("Bẫy Gai (Spike) tạo thành công");
}

Spike::~Spike()
{
    AXLOG("Bẫy Gai (Spike) bị huỷ");
}

bool Spike::init(const std::string& spriteFile)
{
    if (!Sprite::initWithFile(spriteFile))  // Thay đổi từ Node::init() sang Sprite::initWithFile()
        return false;

    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(0x02);
        trapBody->setCollisionBitmask(0x01);
        trapBody->setContactTestBitmask(0x01);
        this->setPhysicsBody(trapBody);
    }

    this->setTag(static_cast<int>(TrapType::Spike));
    return true;
}

void Spike::dealDamage(ax::Node* player)
{
    if (!isActive)
        return;
    AXLOG("Bẫy Gai (Spike) gây %d sát thương!", damage);
}

void Spike::activateTrap()
{
    if (isActive)
        return;
    isActive = true;
    AXLOG("Bãy Gai (Spike) đã kích hoạt!");
}

void Spike::deactivateTrap()
{
    isActive = false;
    AXLOG("Bãy Gai (Spike) đã dừng!");
}
