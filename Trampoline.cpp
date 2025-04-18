#include "axmol.h"
#include "Trampoline.h"

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
}

bool Trampoline::init(const std::string& spriteFile)
{
    if (!Sprite::initWithFile(spriteFile))  // Thay đổi từ Node::init() sang Sprite::initWithFile()
        return false;

    // Đặt AnchorPoint tại chính giữa, dưới chân (nếu Trampoline hướng lên trên)
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
    this->setTag(static_cast<int>(TrapType::Trampoline));
    return true;
}

void Trampoline::dealDamage(ax::Node* player)
{
    if (!isActive)
        return;

    AXLOG("Bẫy (Trampoline) gây %d sát thương!", damage);
    AXLOG("Đối tượng bị đẩy ngược lại");

    // Chèn âm thanh
    int audio = AudioEngine::play2d("sound_game/mechanic/hydraulic-cut.wav", false, 1.0f);

    // Lấy góc xoay hiện tại của Trampoline
    float veloRot = this->getRotation();

    // Thực hiện phép xoay Vector vận tốc tác động mặc định
    Vec2 applyVelo = this->DefltApplyVelo.rotateByAngle(Vec2::ZERO, AX_DEGREES_TO_RADIANS(-veloRot)); // Dấu trừ, vì ở đây quay theo quy tắc toán học, khác ->setRotation()

    // Hiển thị applyVelo
    AXLOG("applyVelo: (%.0f, %.0f)", applyVelo.x, applyVelo.y);

    // Kiểm tra nếu player có PhysicsBody
    auto playerBody = player->getPhysicsBody();
    if (playerBody)
    {
        playerBody->applyImpulse(applyVelo * playerBody->getMass());
    }
    else
    {
        AXLOG("Cảnh báo: Player không có PhysicsBody! Không thể tác động lực.");
    }
}


void Trampoline::activateTrap()
{
    if (isActive)
        return;
    isActive = true;
    AXLOG("Bãy (Trampoline) đã kích hoạt!");

    // Load sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Trampoline/Trampoline_Jump.plist");

    // Set sprite frame đầu tiên thay vì tạo sprite mới
    this->setSpriteFrame("Trampoline_Jump_1.png");

    // Tạo animation từ các frame
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 8; i++)
    {
        std::string frameName = "Trampoline_Jump_" + std::to_string(i) + ".png";
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
    auto animation = Animation::createWithSpriteFrames(frames, 1.0f/8);
    auto animate   = Animate::create(animation);

    // Chạy Animation
    this->runAction(RepeatForever::create(animate));

    // Hẹn thời gian để tắt bẫy sau 1.0 giây
    this->scheduleOnce([this](float) { this->deactivateTrap(); }, 1.0f, "deactivate_trap");

}

void Trampoline::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (Trampoline) đã dừng!");
    // Dừng tất cả animation đang chạy
    this->stopAllActions();

    this->setTexture("Traps/Trampoline/Trampoline_Idle.png");
}



