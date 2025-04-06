#include "axmol.h"
#include "RockHead.h"

USING_NS_AX;

RockHead::RockHead() : Trap()
{
    damage    = 0;
    isActive  = true;
    isDynamic = false;
    amplitude = 0.0f;
    frequency = 0.0f;
    AXLOG("Bẫy (RockHead) tạo thành công");
}
RockHead::~RockHead()
{
    AXLOG("Bẫy (RockHead) bị huỷ");
}

bool RockHead::init(const std::string& spriteFile)
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
    this->setTag(static_cast<int>(TrapType::RockHead));
    
    return true;
}

void RockHead::dealDamage(ax::Node* player)
{
    if (!isActive)
        return;

    AXLOG("Bẫy (RockHead) gây %d sát thương!", damage);
    
}

void RockHead::activateTrap(std::string axis)
{
    if (isActive == false)
        isActive = true;

    AXLOG("Bãy (RockHead) đã kích hoạt!");

    startBehavior(axis);  // Gọi phương thức thực hiện hành vi
}

void RockHead::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (RockHead) đã dừng!");
}

ax::Animation* RockHead::getAnimation(std::string animName)
{
    // Load sprite sheet nếu chưa có
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Rock Head/RockHead_Blink.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Rock Head/RockHead_LeftHit.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Rock Head/RockHead_RightHit.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Rock Head/RockHead_TopHit.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Traps/Rock Head/RockHead_BottomHit.plist");

    // Tạo các vectorFrames từ các frame
    // 1. blink
    Vector<SpriteFrame*> blinkFrames;
    for (int i = 1; i <= 4; i++)
    {
        std::string frameName = "RockHead_Blink_" + std::to_string(i) + ".png";
        auto frame            = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            blinkFrames.pushBack(frame);
        }
        else
        {
            AXLOG("Cảnh báo: Không tìm thấy frame %s!", frameName.c_str());
        }
    }

    if (blinkFrames.empty())
    {
        AXLOG("Lỗi: Không tìm thấy bất kỳ frame nào cho blinkAnim!");
        return nullptr;
    }

    // 2. leftHit
    Vector<SpriteFrame*> leftHitFrames;
    for (int i = 1; i <= 4; i++)
    {
        std::string frameName = "RockHead_LeftHit_" + std::to_string(i) + ".png";
        auto frame            = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            leftHitFrames.pushBack(frame);
        }
        else
        {
            AXLOG("Cảnh báo: Không tìm thấy frame %s!", frameName.c_str());
        }
    }

    if (leftHitFrames.empty())
    {
        AXLOG("Lỗi: Không tìm thấy bất kỳ frame nào cho leftHitAnim!");
        return nullptr;
    }

    // 3. rightHit
    Vector<SpriteFrame*> rightHitFrames;
    for (int i = 1; i <= 4; i++)
    {
        std::string frameName = "RockHead_RightHit_" + std::to_string(i) + ".png";
        auto frame            = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            rightHitFrames.pushBack(frame);
        }
        else
        {
            AXLOG("Cảnh báo: Không tìm thấy frame %s!", frameName.c_str());
        }
    }

    if (rightHitFrames.empty())
    {
        AXLOG("Lỗi: Không tìm thấy bất kỳ frame nào cho rightHitAnim!");
        return nullptr;
    }

    // 4. topHit
    Vector<SpriteFrame*> topHitFrames;
    for (int i = 1; i <= 4; i++)
    {
        std::string frameName = "RockHead_TopHit_" + std::to_string(i) + ".png";
        auto frame            = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            topHitFrames.pushBack(frame);
        }
        else
        {
            AXLOG("Cảnh báo: Không tìm thấy frame %s!", frameName.c_str());
        }
    }

    if (topHitFrames.empty())
    {
        AXLOG("Lỗi: Không tìm thấy bất kỳ frame nào cho topHitAnim!");
        return nullptr;
    }

    // 5. bottomHit
    Vector<SpriteFrame*> bottomHitFrames;
    for (int i = 1; i <= 4; i++)
    {
        std::string frameName = "RockHead_BottomHit_" + std::to_string(i) + ".png";
        auto frame            = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
        if (frame)
        {
            bottomHitFrames.pushBack(frame);
        }
        else
        {
            AXLOG("Cảnh báo: Không tìm thấy frame %s!", frameName.c_str());
        }
    }

    if (bottomHitFrames.empty())
    {
        AXLOG("Lỗi: Không tìm thấy bất kỳ frame nào cho bottomHitAnim!");
        return nullptr;
    }

    // Tạo Animation
    auto blinkAnim = Animation::createWithSpriteFrames(blinkFrames, 0.1f);
    auto leftHitAnim = Animation::createWithSpriteFrames(leftHitFrames, 0.1f);
    auto rightHitAnim = Animation::createWithSpriteFrames(rightHitFrames, 0.1f);
    auto topHitAnim   = Animation::createWithSpriteFrames(topHitFrames, 0.1f);
    auto bottomHitAnim = Animation::createWithSpriteFrames(bottomHitFrames, 0.1f);

    if (animName == "blinkAnim")
        return blinkAnim;
    else if (animName == "leftHitAnim")
        return leftHitAnim;
    else if (animName == "rightHitAnim")
        return rightHitAnim;
    else if (animName == "topHitAnim")
        return topHitAnim;
    else if (animName == "bottomHitAnim")
        return bottomHitAnim;
    else
    {
        AXLOG("Vui lòng truyền vào đúng cú pháp: 'blinkAnim', 'leftHitAnim', 'rightHitAnim', 'topHitAnim', 'bottomHitAnim'");
        return nullptr;
    }
}

void RockHead::startBehavior(std::string axis)
{
    AXLOG("RockHead bắt đầu hành vi của mình!");

    // Lấy hoạt ảnh:
    auto blinkAnim = this->getAnimation("blinkAnim");
    auto leftHitAnim = this->getAnimation("leftHitAnim");
    auto rightHitAnim = this->getAnimation("rightHitAnim");
    auto topHitAnim   = this->getAnimation("topHitAnim");
    auto bottomHitAnim = this->getAnimation("bottomHitAnim");


    // Tính chu kỳ
    float period = 1.0f / this->frequency;

    auto moveLeft  = MoveBy::create(period/4, Vec2(-amplitude, 0));
    auto moveRight = MoveBy::create(period/4, Vec2(+amplitude, 0));
    auto moveUp    = MoveBy::create(period / 4, Vec2(0, +amplitude));
    auto moveDown = MoveBy::create(period / 4, Vec2(0, -amplitude));

    auto blink    = Animate::create(blinkAnim);

    auto leftHit  = Animate::create(leftHitAnim);
    auto rightHit = Animate::create(rightHitAnim);
    auto topHit   = Animate::create(topHitAnim);
    auto bottomHit = Animate::create(bottomHitAnim);

    auto wait = DelayTime::create(0.2f);  // dừng 1 giây

    auto moveLeftWithBlink  = Spawn::createWithTwoActions(moveLeft, blink);
    auto moveRightWithBlink = Spawn::createWithTwoActions(moveRight, blink);
    auto moveUpWithBlink    = Spawn::createWithTwoActions(moveUp, blink);
    auto moveDownWithBlink  = Spawn::createWithTwoActions(moveDown, blink);

    auto horizontalSeq =
        Sequence::create(moveLeftWithBlink, leftHit, wait, moveRightWithBlink, rightHit, wait, nullptr);
    auto verticalSeq =
        Sequence::create(moveUpWithBlink, topHit, wait, moveDownWithBlink, bottomHit, wait, nullptr);

    // Lặp lại mãi mãi
    if (axis == "horizontal")
        this->runAction(RepeatForever::create(horizontalSeq));
    else if (axis == "vertical")
        this->runAction(RepeatForever::create(verticalSeq));
    else
    {
        AXLOG("Vui lòng truyền vào đúng cú pháp: 'horizontal', hoặc 'vertical'");
        return;
    }
}

void RockHead::stopBehavior()
{
    AXLOG("RockHead bắt đầu hành vi của mình!");
    this->stopAllActions();  // Dừng tất cả hành động (bao gồm cả dao động)
}

//void RockHead::checkWallCollision()
//{
//    // Kiểm tra nếu không còn tiếp xúc với tường
//    if (!this->getPhysicsBody()->isContactTestBitmask() & 0x03)
//    {
//        AXLOG("RockHead đã rơi khỏi tường!");
//        this->stopOscillation();
//        this->getPhysicsBody()->setDynamic(true);  // Bật rơi tự do
//    }
//}

void RockHead::setAmplitude(float _amp)
{
    this->amplitude=_amp;
}

float RockHead::getAmplitude()
{
    return amplitude;
}

void RockHead::setFrequency(float _freq)
{
    this->frequency = _freq;
}

float RockHead::getFrequency()
{
    return frequency;
}
