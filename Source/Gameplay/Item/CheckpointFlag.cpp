#include "CheckpointFlag.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

using namespace ax;

CheckpointFlag* CheckpointFlag::createCheckpoint()
{
    CheckpointFlag *ret = new CheckpointFlag();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
};

bool CheckpointFlag::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    this->initWithTexture(SpriteManager::getInstance().getTextureByName("Checkpoint_NoFlag"));
    // Tạo action
    auto flagOutFrames = SpriteManager::getInstance().createVectorSpriteFrame("Checkpoint_FlagOut_%d.png", 26);
    auto flagOutAnimation = Animation::createWithSpriteFrames(flagOutFrames, 1.0f/35);
    auto _flagOutAction = Repeat::create(Animate::create(flagOutAnimation), 1);

    auto flagIdleFrames = SpriteManager::getInstance().createVectorSpriteFrame("Checkpoint_FlagIdle_%d.png", 10);
    auto flagIdleAnimation = Animation::createWithSpriteFrames(flagIdleFrames, 1.0f/20);
    _flagIdleAction = RepeatForever::create(Animate::create(flagIdleAnimation));
    _flagIdleAction->retain();

    _flagAction = Sequence::createWithTwoActions(
        _flagOutAction,
        CallFunc::create([=](){ this->runAction(_flagIdleAction); })
    );
    _flagAction->retain();

    // Tạo physicsbody
    auto itemBody = PhysicsBody::createBox(Vec2(12, 46), PhysicsMaterial(0.1f, 0.0f, 0.0f));
    itemBody->setDynamic(false);
    itemBody->setPositionOffset(Vec2(-8, -9));
    itemBody->setCategoryBitmask(ObjectBitmask::ChechpointFlag);
    itemBody->setCollisionBitmask(ObjectBitmask::Ground);
    itemBody->setContactTestBitmask(ObjectBitmask::Player);

    this->addComponent(itemBody);
    this->setName(nameItem);

    return true;
}

void CheckpointFlag::flagOut()
{
    SoundManager::playEffect(AudioPaths::CHECKPOINT);
    this->stopAllActions();
    this->runAction(_flagAction);  
}

void CheckpointFlag::updateItem()
{
    auto currentLevel = SceneManager::getCurrentScene();
    if (currentLevel == SceneType::Level_4_Scene)
    {
        AXLOG("Win!");
        auto transfer = CallFunc::create([=](){ SceneManager::create_and_replace_currentScene(SceneType::GameOver_Scene); });
        this->flagOut();
        this->runAction(Sequence::create(
            DelayTime::create(3.0f),
            transfer,
            nullptr
        ));
        return;
    }        
    auto nextLevel = static_cast<SceneType>(static_cast<int>(currentLevel) + 1);  
    auto transferLevel = CallFunc::create([=](){
        SceneManager::create_and_replace_currentScene(nextLevel);
    });
    this->flagOut();
    this->runAction(Sequence::create(
        DelayTime::create(1.5f),
        transferLevel,
        nullptr
    ));
}

// Hàm hủy
CheckpointFlag::~CheckpointFlag()
{
    AX_SAFE_RELEASE(_flagIdleAction);
    AX_SAFE_RELEASE(_flagAction);
}