#include <HeartLives.h>
#include <SpriteManager.h>

using namespace ax;

HeartLives* HeartLives::instance = nullptr;

HeartLives* HeartLives::getInstance()
{
    if (!instance)
    {
        instance = new HeartLives();
        if (instance && instance->init())
        {
            instance->retain(); // giữ lại giúp không tự bị xóa khi hết scene
            return instance;
        }
        delete instance;
        instance = nullptr;
        return nullptr;
    }
    return instance;
}

bool HeartLives::init()
{
    if (!Node::init()) 
    {
        return false;
    }
    // Tạo trái tim
    int original_quantity = quantity;
    for (int i = 0; i < original_quantity; i++)
    {
        addAHeart();
    }
    return true;
}

void HeartLives::addAHeart()
{
    // Tạo 1 trái tim mới
    auto idleFrames = SpriteManager::getInstance().createVectorSpriteFrame("HeartIdle_%d.png", 8);
    idleFrames.pushBack(idleFrames.front());
    auto heartSprite = Sprite::createWithSpriteFrame(idleFrames.front());
    auto idleAnimation = Animation::createWithSpriteFrames(idleFrames, 1.0f/10);
    auto idleAnimate = Animate::create(idleAnimation); // tạo 1 action
    auto delayTime = DelayTime::create(15.0f); // delay time
    auto actionSequence = Sequence::create(idleAnimate, delayTime, nullptr); // tạo chuỗi
    auto actionHeart =  RepeatForever::create(actionSequence); // lặp lại chuỗi
    heartSprite->runAction(actionHeart);
    heartSprite->setScale(2.0f);
    this->addChild(heartSprite);

    // Tính toán vị trí và cho vào vector để lưu trữ
    if (vt_hearts.empty())
    {
        heartSprite->setPosition(Vec2(30, 330));
    }
    else
    {
        heartSprite->setPosition(vt_hearts.back()->getPosition() + Vec2(30, 0));
    }
    vt_hearts.pushBack(heartSprite);
    quantity++;
}

void HeartLives::deleteAHeart()
{
    // Xóa 1 trái tim trước
    if (vt_hearts.empty())
    {
        return;
    }
    
    this->removeChild(vt_hearts.back());
    vt_hearts.popBack();
    quantity--;
}

void HeartLives::deleteInstance()
{
    if (instance)
    {
        instance->release();
        instance = nullptr;
    }
}