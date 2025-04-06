#include "axmol.h"
#include "Fan.h"

USING_NS_AX;

Fan::Fan() : Trap()
{
    damage    = 0;
    isActive  = false;
    isDynamic = false;
    windZone  = nullptr;

    AXLOG("Bẫy (Fan) tạo thành công");
}
Fan::~Fan()
{
    AXLOG("Bẫy (Fan) bị huỷ");
}

bool Fan::init(const std::string& spriteFile)
{
    if (!Sprite::initWithFile(spriteFile))  // Thay đổi từ Node::init() sang Sprite::initWithFile()
        return false;

    auto windSize = Size(this->getContentSize().width, 100);  // kích thước vùng gió (tuỳ bạn)
    Vec2 forceDir = DeftForceVector;                          // có thể tuỳ chỉnh theo chiều quay quạt

    windZone = new WindZone(windSize, forceDir);
    windZone->setAnchorPoint(Vec2(0.5, 0));
    windZone->setPosition(this->getContentSize().width / 2,
                          this->getContentSize().height);  // ví dụ: vùng gió phía trên


    // Đặt AnchorPoint tại chính giữa, dưới chân (nếu Fan hướng lên trên)
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
    this->setTag(static_cast<int>(TrapType::Fan));

    this->addChild(windZone);

    return true;
}


void Fan::dealDamage(ax::Node* player)
{
    if (!isActive)
        return;

    // AXLOG("ĐI VÀO VÙNG ẢNH HƯỞNG");

    AXLOG("Chạm vào bẫy quạt");

    // Kiểm tra nếu player có PhysicsBody
    /*auto playerBody = player->getPhysicsBody();
    if (playerBody)
    {
        windZone->doAction();
    }
    else
    {
        AXLOG("Cảnh báo: Player không có PhysicsBody! Không thể tác động lực.");
    }*/

    // Hiện chưa tìm được cách xử lý sự kiện khi nhân vật vào vùng ảnh hưởng
}

void Fan::activateTrap()
{
    if (isActive)
        return;
    isActive = true;
    AXLOG("Bãy (Fan) đã kích hoạt!");
    // Load sprite sheet
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Fan/Fan_On.plist");

    // Set sprite frame đầu tiên thay vì tạo sprite mới
    this->setSpriteFrame("Fan_On_1.png");

    // Tạo animation từ các frame
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 4; i++)
    {
        std::string frameName = "Fan_On_" + std::to_string(i) + ".png";
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
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate   = Animate::create(animation);

    // Chạy Animation
    this->runAction(RepeatForever::create(animate));

    windZone->doAction();

}

void Fan::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (Fan) đã dừng!");
    // Dừng tất cả animation đang chạy
    this->stopAllActions();

    this->setTexture("Traps/Fan/Fan_Off.png");
}

WindZone* Fan::getWindZone()
{
    return windZone;
}
