#include "Box.h"
#include "SpriteManager.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "SoundManager.h"

using namespace ax;

bool Box::init()
{
    if (!Sprite::init())
    {
        return false;
    }
    // Tạo hành động đứng yên Idle
    auto idleFrames = SpriteFrame::createWithTexture(SpriteManager::getInstance().getTextureByName(nameTextureBox), Rect(0, 0, 28, 24));
    this->initWithSpriteFrame(idleFrames);
    auto idleAnimation = Animation::createWithSpriteFrames(Vector<SpriteFrame*> {idleFrames}, 1.0f);
    _idleAction = RepeatForever::create(Animate::create(idleAnimation));
    _idleAction->retain();
    this->runAction(_idleAction);

    // Tạo hành động hit
    auto hitFrames = SpriteManager::getInstance().createVectorSpriteFrame(boxHitPNG, numberOfBoxHit);
    auto hitAnimation = Animation::createWithSpriteFrames(hitFrames, 1.0f/20);
    _hitAction = Sequence::create(
        Repeat::create(Animate::create(hitAnimation), 1),
        CallFunc::create([=] { 
            this->idle(); }),
        nullptr
    );
    _hitAction->retain();

    // Tạo physicsbody cho box
    auto itemBody = PhysicsBody::createBox(Vec2(20, 20), PhysicsMaterial(0.5f, 0.0f, 0.0f));
    itemBody->setDynamic(false);
    itemBody->setCategoryBitmask(ObjectBitmask::Box);
    itemBody->setCollisionBitmask(ObjectBitmask::Player);
    itemBody->setContactTestBitmask(ObjectBitmask::Player);

    this->addComponent(itemBody);
    this->setName(nameItem);

    // Tạo node chứa các fragment
    fragmentNode = Node::create();
    auto breakFrames = SpriteManager::getInstance().createVectorSpriteFrame(boxBreakPNG, 4);

    for (int i = 0; i < 4; i++)
    {
        auto fragment = Sprite::createWithSpriteFrame(breakFrames[i]);
        auto physicsBodyF = PhysicsBody::createBox(Vec2(12, 10), PhysicsMaterial(0.1f, 0.0f, 0.1f)); // Tạo physicbody từng mảnh
        physicsBodyF->setDynamic(true);
        physicsBodyF->setEnabled(false);
        physicsBodyF->setCategoryBitmask(ObjectBitmask::Box_Fragment);
        physicsBodyF->setCollisionBitmask(ObjectBitmask::Ground|ObjectBitmask::Sand|ObjectBitmask::Mud|ObjectBitmask::Ice);
        fragment->addComponent(physicsBodyF);
        fragmentNode->addChild(fragment);       
    }
    fragmentNode->setVisible(false);
    this->addChild(fragmentNode);

    return true;
}

// Hàm gọi action
void Box::idle()
{
    isHit = false;
    this->stopAllActions();
    this->runAction(_idleAction);
}

void Box::hit()
{
    isHit = true;
    this->stopAllActions();
    this->runAction(_hitAction);
}

// Phá hủy box
void Box::breakBox()
{   
    this->setOpacity(0); // ẩn đi
    this->getPhysicsBody()->setEnabled(false); // Tắt physicbody node chính
    SoundManager::playEffect(AudioPaths::BROKEN_BOX);
    fragmentNode->setVisible(true);
    for (auto& child : fragmentNode->getChildren())
    {
        auto fragmentBody = child->getPhysicsBody();
        if (fragmentBody)
        {
            fragmentBody->setEnabled(true);
            fragmentBody->setVelocity(Vec2(random(-200, 200), random(150, 250)));
        }

        auto fadeOut = FadeOut::create(3.0f); // Làm mờ dần
        auto remove = RemoveSelf::create(); // Xóa node
        auto sequence = Sequence::create(fadeOut, remove, nullptr);
        child->runAction(sequence);       
    }

    auto removeBox = CallFunc::create([=] {
       this->removeFromParent(); // Xóa box khỏi scene
    });
    this->runAction(Sequence::create(DelayTime::create(4.0f), removeBox, nullptr));
}

void Box::updateItem()
{
    if (isHit)
        return;
    if (1 < getCurrentHPItem())
    {
        SoundManager::playEffect(AudioPaths::HIT_BOX);
        minusCurrentHPItem(1);
        this->hit();
    }
    else if (getCurrentHPItem() <= 1)
    {
        AXLOG("%s break", getNameItem().c_str());
        this->hit();
        this->breakBox();
        GameManager::getInstance().spawnFruitsAt(this->getPosition(), this->getMaxHPItem() + 1);
    }
}

// Hàm hủy
Box::~Box()
{
    AX_SAFE_RELEASE(_idleAction); // Xóa hành động vì đã lưu vào bộ nhớ ở trên
    AX_SAFE_RELEASE(_hitAction);
}