#include "axmol.h"
#include "Fire.h"

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
}

bool Fire::init(const std::string& spriteFile)
{
    if (!Sprite::initWithFile(spriteFile))  // Thay đổi từ Node::init() sang Sprite::initWithFile()
        return false;

    // Đặt AnchorPoint tại chính giữa, dưới chân (nếu Fire hướng lên trên)
    this->setAnchorPoint(Vec2(0.5, 0));

    // Tạo PhysicsBody mới
    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(0x02);
        trapBody->setCollisionBitmask(0x01);
        trapBody->setContactTestBitmask(0x01);

        // Gán PhysicsBody vào Sprite
        this->setPhysicsBody(trapBody);
    }
    this->setTag(static_cast<int>(TrapType::Fire));
    return true;
}

void Fire::dealDamage(ax::Node* player)
{
    if (!isActive)
        return;

    AXLOG("Bẫy (Fire) gây %d sát thương!", damage);
    // Chèn âm thanh
    int audio = AudioEngine::play2d("sound_game/mechanic/fire-cut.wav", false, 0.8f);

}


void Fire::activateTrap()
{
    if (isActive)
        return;
    isActive = true;
    AXLOG("Bãy (Fire) đã kích hoạt!");
    // Load sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Fire/Fire_On.plist");

    // Set sprite frame đầu tiên thay vì tạo sprite mới
    this->setSpriteFrame("Fire_On_1.png");

    // Tạo animation từ các frame
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 3; i++)
    {
        std::string frameName = "Fire_On_" + std::to_string(i) + ".png";
        auto frame            = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            frames.pushBack(frame);
        }
        else
        {
            AXLOG("Cảnh báo: Không tìm thấy frame %s!", frameName.c_str());
        }
    }

    // Nếu không có frame nào, thoát sớm
    if (frames.empty())
    {
        AXLOG("Lỗi: Không tìm thấy bất kỳ frame nào cho animation!");
        return;
    }

    // Tạo Animation
    auto animation = Animation::createWithSpriteFrames(frames, 1.0f/3);
    auto animate   = Animate::create(animation);

    // Chạy Animation
    this->runAction(RepeatForever::create(animate));

    // Hẹn thời gian để tắt bẫy sau 1.0 giây
    this->scheduleOnce([this](float) { this->deactivateTrap(); }, 1.0f, "deactivate_trap");
}

void Fire::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (Fire) đã dừng!");
    // Dừng tất cả animation đang chạy
    this->stopAllActions();

    this->setTexture("Traps/Fire/Fire_Off.png");
}



