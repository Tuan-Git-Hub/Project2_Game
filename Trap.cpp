#include "Trap.h"

USING_NS_AX;

Trap::Trap()
{
    trapBody  = nullptr;
    damage    = 1;
    isActive  = false;
    isDynamic = false;
}

Trap::Trap(bool _isActv, int dmg, bool _isDync)
{
    trapBody = nullptr;
    damage    = dmg;
    isActive  = _isActv;
    isDynamic = _isDync;
}

Trap::~Trap()
{
    AXLOG("Bẫy bị huỷ");
}

bool Trap::init(const std::string& spriteFile)
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

    this->setTag(static_cast<int>(TrapType::GenericTrap));
    return true;
}

void Trap::activateTrap()
{
    if (isActive)
        return;
    isActive = true;
    AXLOG("Bãy đã kích hoạt!");
}

void Trap::deactivateTrap()
{
    isActive = false;
    AXLOG("Bãy đã dừng!");
}

void Trap::dealDamage(Node* player)
{
    if (!isActive)
        return;
    AXLOG("Bãy gây %d sát thương!", damage);
}

PhysicsBody* Trap::getTrapBody()
{
    return trapBody;
}
void Trap::setTrapBody(PhysicsBody* body)
{
    trapBody = body;
    this->setPhysicsBody(trapBody);
}

int Trap::getDamage()
{
    return damage;
}
void Trap::setDamage(int dmg)
{
    damage = dmg;
}

bool Trap::getActiveStatus()
{
    return isActive;
}
void Trap::setActiveStatus(bool active)
{
    isActive = active;
}

bool Trap::getDynamicStatus()
{
    return isDynamic;
}

void Trap::setDynamicStatus(bool _dync)
{
    isDynamic = _dync;
}
