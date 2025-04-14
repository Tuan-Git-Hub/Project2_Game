#include "Spike.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"

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

bool Spike::init()
{
    if (!Sprite::initWithTexture(SpriteManager::getInstance().getTextureByName("spikeTrap"))) 
        return false;

    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(ObjectBitmask::SpikeTrap);
        trapBody->setCollisionBitmask(ObjectBitmask::Player|ObjectBitmask::Ground);
        trapBody->setContactTestBitmask(ObjectBitmask::Player);
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
