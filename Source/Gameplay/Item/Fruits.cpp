#include "Fruits.h"
#include "SpriteManager.h"
#include "GameObjectManager.h"
#include "GameManager.h"

using namespace ax;

Fruits* Fruits::createFruit()
{
    Fruits *ret = new Fruits();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
};

bool Fruits::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    // Lấy random fruit
    int index = random(0, 7);
    fruitsIdlePNG = fruits[index];
    // Tạo action idle
    auto idleFrames = SpriteManager::getInstance().createVectorSpriteFrame(fruitsIdlePNG, 17);
    this->initWithSpriteFrame(idleFrames.front());
    auto idleAnimation = Animation::createWithSpriteFrames(idleFrames, 1.0f/15);
    this->_idleAction =  RepeatForever::create(Animate::create(idleAnimation));
    _idleAction->retain(); // Lưu lại hành động, tránh phải tạo lần sau, vì dùng nhiều
    this->runAction(_idleAction);

    // Tạo action collected
    auto collectedFrames = SpriteManager::getInstance().createVectorSpriteFrame(fruitsCollectedPNG, 6);
    auto collectedAnimation = Animation::createWithSpriteFrames(collectedFrames, 1.0f/20);
    _collectedAction = Repeat::create(Animate::create(collectedAnimation), 1);
    _collectedAction->retain();

    // Tạo physicsbody
    auto itemBody = PhysicsBody::createBox(Vec2(13, 13), PhysicsMaterial(0.1f, 0.0f, 0.15f));
    itemBody->setDynamic(false);
    itemBody->setCategoryBitmask(ObjectBitmask::Fruits);
    itemBody->setCollisionBitmask(ObjectBitmask::Ground|ObjectBitmask::Sand|ObjectBitmask::Mud|ObjectBitmask::Ice);
    itemBody->setContactTestBitmask(ObjectBitmask::Player);
    itemBody->setRotationEnable(false); // Không cho phép vật thể xoay

    this->addComponent(itemBody);
    this->setName(nameItem);

    return true;
}

// Chạm vào
void Fruits::collected()
{
    this->stopAllActions();
    this->runAction(_collectedAction);
    auto addPoint = CallFunc::create([=] {
        GameManager::getInstance().addPointPlayer();
    });
    auto removeFruit = CallFunc::create([=] {
        this->removeFromParent(); // Xóa box khỏi scene
    });
    this->runAction(Sequence::create(addPoint, DelayTime::create(0.3f), removeFruit, nullptr));
}

void Fruits::updateItem()
{
    this->collected();
}


// Hàm hủy
Fruits::~Fruits()
{
    AX_SAFE_RELEASE(_idleAction); // Xóa hành động vì đã lưu vào bộ nhớ ở trên
    AX_SAFE_RELEASE(_collectedAction);
}