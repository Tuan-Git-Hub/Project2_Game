#include "axmol.h"
#include "Saw.h"

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

bool Saw::init(const std::string& spriteFile)
{
    if (!Sprite::initWithFile(spriteFile))  // Thay đổi từ Node::init() sang Sprite::initWithFile()
        return false;

    // Tạo PhysicsBody mới
    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(0x02);
        trapBody->setCollisionBitmask(0x01 /*Player*/ | 0x03 /*Tường*/);
        trapBody->setContactTestBitmask(0x01 | 0x03);

        // Gán PhysicsBody vào Sprite
        this->setPhysicsBody(trapBody);
    }
    this->setTag(static_cast<int>(TrapType::Saw));
    
    return true;
}

void Saw::dealDamage(ax::Node* player)
{
    if (!isActive)
        return;

    AXLOG("Bẫy (Saw) gây %d sát thương!", damage);
    
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

    // Chèn âm thanh
    // int audio = AudioEngine::play2d("sound_game/mechanic/circular-saw-cut.wav", true, 0.4f);

    // Load sprite sheet nếu chưa có
    if (!SpriteFrameCache::getInstance()->isSpriteFramesWithFileLoaded("Traps/Saw/Saw_On.plist"))
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Saw/Saw_On.plist");

    // Tạo animation từ các frame
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= 8; i++)
    {
        std::string frameName = "Saw_On_" + std::to_string(i) + ".png";
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

    if (frames.empty())
    {
        AXLOG("Lỗi: Không tìm thấy bất kỳ frame nào cho animation!");
        return;
    }

    float period = 1.0f / this->frequency;

    // Tạo Animation
    auto animation = Animation::createWithSpriteFrames(frames, (period/8.0f));
    auto animate   = Animate::create(animation);

    // Chạy Animation lặp vô hạn
    this->runAction(RepeatForever::create(animate));
}

void Saw::startOscillation()
{
    AXLOG("Saw bắt đầu dao động điều hòa!");

    // Tạo hành động dao động

    // Tính chu kỳ
    float period = 1.0f / this->frequency;

    auto moveRight = MoveBy::create((period / 4.0f), Vec2(amplitude, 0));  // Di chuyển sang phải
    auto moveLeft  = MoveBy::create((period / 4.0f), Vec2(-amplitude, 0));  // Di chuyển sang trái

    // Tạo hành động tuần hoàn
    auto oscillate = RepeatForever::create(Sequence::create(moveRight, moveLeft, moveLeft, moveRight, nullptr));

    // Chạy hành động
    this->runAction(oscillate);
}

void Saw::stopOscillation()
{
    AXLOG("Saw dừng dao động điều hòa!");
    this->stopAllActions();  // Dừng tất cả hành động (bao gồm cả dao động)
}

//void Saw::checkWallCollision()
//{
//    // Kiểm tra nếu không còn tiếp xúc với tường
//    if (!this->getPhysicsBody()->isContactTestBitmask() & 0x03)
//    {
//        AXLOG("Saw đã rơi khỏi tường!");
//        this->stopOscillation();
//        this->getPhysicsBody()->setDynamic(true);  // Bật rơi tự do
//    }
//}

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
