#include "Fan.h"
#include "GameObjectManager.h"
#include "SpriteManager.h"
#include "SoundManager.h"

USING_NS_AX;

Fan::Fan() : Trap()
{
    damage    = 0;
    isActive  = false;
    isDynamic = false;

    AXLOG("Bẫy (Fan) tạo thành công");
}
Fan::~Fan()
{
    AXLOG("Bẫy (Fan) bị huỷ");
    AX_SAFE_RELEASE(actTrap);
}

bool Fan::init()
{
    if (!Sprite::initWithTexture(SpriteManager::getInstance().getTextureByName("fan")))
        return false;

    // Đặt AnchorPoint tại chính giữa, dưới chân (nếu Fan hướng lên trên)
    this->setAnchorPoint(Vec2(0.5, 0));

    // Tạo PhysicsBody mới
    trapBody = PhysicsBody::createBox(this->getContentSize());
    if (trapBody)
    {
        trapBody->setDynamic(isDynamic);
        trapBody->setCategoryBitmask(ObjectBitmask::Fan);
        trapBody->setCollisionBitmask(ObjectBitmask::Ground);
        trapBody->setContactTestBitmask(ObjectBitmask::Player);

        // Gán PhysicsBody vào Sprite
        this->setPhysicsBody(trapBody);
    }
    this->setTag(static_cast<int>(TrapType::Fan));

    // Load sprite sheet
    auto frames = SpriteManager::getInstance().createVectorSpriteFrame("Fan_On_%d.png", 4);

    // Tạo Animation
    auto animation = Animation::createWithSpriteFrames(frames, 0.05f);
    auto animate   = Animate::create(animation);

    // Chạy Animation
    actTrap =  RepeatForever::create(animate);
    actTrap->retain();

    // Tạo vùng gió winzone
    winZone = Node::create();
    winZone->setContentSize(Vec2(this->getContentSize().width, 200));
    winZone->setAnchorPoint(Vec2(0.5, 0));
    winZone->setPosition(Vec2(this->getContentSize().width / 2, 0.0f));

    auto effect = ParticleSystemQuad::create();
    effect->setTexture(SpriteManager::getInstance().getTextureByName("dustParticle"));
    // Thiết lập thông số hạt
    effect->setEmissionRate(20); // số hạt mỗi giây
    effect->setLife(1.2f);       // thời gian sống của mỗi hạt (giây)
    effect->setLifeVar(0.2f);    // biến động thời gian sống
    effect->setSpeed(200);       // tốc độ bay
    effect->setSpeedVar(20);     // biến động tốc độ
    effect->setGravity(ax::Vec2(0, 0)); // không trọng lực
    effect->setStartColor(ax::Color4F(1, 1, 1, 0.7f)); // màu ban đầu
    effect->setEndColor(ax::Color4F(1, 1, 1, 0.0f));   // màu khi biến mất (alpha = 0)
    effect->setStartSize(8.0f); // kích thước ban đầu
    effect->setEndSize(5.0f);
    effect->setAngle(90);        // góc bay lên (90 độ = thẳng lên)
    effect->setAngleVar(3);     // dao động góc
    effect->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2)); // vị trí ban đầu của hạt
    effect->setPosVar(Vec2(12, 0));  // biến động vùng xuất hiện
    effect->setDuration(-1);
    winZone->addChild(effect);

    auto sensorBody = PhysicsBody::createBox(winZone->getContentSize(), ax::PhysicsMaterial(0, 0, 0));
    sensorBody->setDynamic(false);
    sensorBody->setCategoryBitmask(ObjectBitmask::WinZone);           // Gán loại của vùng cảm biến
    sensorBody->setContactTestBitmask(ObjectBitmask::Player);  // Kiểm tra va chạm với người chơi
    sensorBody->setCollisionBitmask(ObjectBitmask::None);          // Không va chạm vật lý
    sensorBody->setEnabled(false);
    winZone->setPhysicsBody(sensorBody);
    winZone->setVisible(false);

    this->addChild(winZone);

    return true;
}

void Fan::activateTrap()
{
    if (isActive)
        return;
    isActive = true;
    AXLOG("Bãy (Fan) đã kích hoạt!");

    this->stopAllActions();
    // Chạy Animation
    this->runAction(actTrap);

    SoundManager::playEffect(AudioPaths::FAN);
    // Bật vùng gió
    winZone->setVisible(true);
    winZone->getPhysicsBody()->setEnabled(true);

    // Hẹn thời gian để tắt bẫy sau 0.3 giây
    this->scheduleOnce([this](float) { this->deactivateTrap(); }, 4.0f, "deactivate_trap");

}

void Fan::deactivateTrap()
{
    if (!isActive)
        return;

    isActive = false;
    AXLOG("Bẫy (Fan) đã dừng!");
    // Dừng tất cả animation đang chạy
    this->stopAllActions();

    // Tắt vùng gió
    winZone->setVisible(false);
    winZone->getPhysicsBody()->setEnabled(false);

    auto spriteFire = SpriteManager::getInstance().getTextureByName("fan");
    this->setTexture(spriteFire);
    this->setTextureRect(Rect(0, 0, spriteFire->getContentSize().width, spriteFire->getContentSize().height));
}