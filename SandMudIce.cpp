#include "SandMudIce.h"

USING_NS_AX;

SandMudIce::SandMudIce() : Trap()
{
    damage    = 0;
    isActive  = true;
    isDynamic = false;
    smiType   = SMIType::Default;

    AXLOG("Bẫy (SandMudIce) tạo thành công");
}

SandMudIce::SandMudIce(SMIType _type)
{
    damage    = 0;
    isActive  = true;
    isDynamic = false;
    smiType = _type;

    AXLOG("Bẫy (SandMudIce) tạo thành công");
}

SandMudIce::~SandMudIce()
{
    AXLOG("Bẫy (SandMudIce) bị huỷ");
}

bool SandMudIce::init(const std::string& spriteFile)
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

    this->setTag(static_cast<int>(TrapType::SandMudIce));
    return true;
}

void SandMudIce::dealDamage(ax::Node* player)
{
    if (!isActive)
        return;
    AXLOG("Bẫy (SandMudIce) gây %d sát thương!", damage);

    Vec2 playerVelo = player->getPhysicsBody()->getVelocity();

    if (playerVelo.x != 0)
    {
        switch (this->smiType)
        {
        case SMIType::Sand:
            // Giảm vận tốc trực tiếp
            playerVelo.x *= 0.8f;  // Giảm 20% mỗi lần chạm
            player->getPhysicsBody()->setVelocity(playerVelo);
            AXLOG("playerVelo (Sand): (%d, %d)", playerVelo.x, playerVelo.y);
            break;
        case SMIType::Mud:
            playerVelo.x = 0;
            player->getPhysicsBody()->setVelocity(playerVelo);
            AXLOG("playerVelo: (%d, %d)", playerVelo.x, playerVelo.y);
            break;
        case SMIType::Ice:
            // Tăng vận tốc trực tiếp
            playerVelo.x *= 1.2f;  // Tăng 20% mỗi lần chạm
            player->getPhysicsBody()->setVelocity(playerVelo);
            AXLOG("playerVelo (Ice): (%d, %d)", playerVelo.x, playerVelo.y);
            break;
        default:
            AXLOG("Lỗi SandMudIce");
            return;
        }
    }
    else
        AudioEngine::stopAll();
}

void SandMudIce::activateTrap()
{
    if (isActive)
        return;
    isActive = true;
    AXLOG("Bãy (SandMudIce) đã kích hoạt!");
}

void SandMudIce::deactivateTrap()
{
    isActive = false;
    AXLOG("Bãy (SandMudIce) đã dừng!");
}
