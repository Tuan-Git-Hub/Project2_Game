#include "HeartLives.h"
#include "SpriteManager.h"
#include "GameManager.h"

using namespace ax;

HeartLives* HeartLives::createHeartLives()
{

    HeartLives* ret = new HeartLives();
    if (ret && ret->init())
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool HeartLives::init()
{
    if (!Node::init()) 
    {
        return false;
    }
    // Tạo trái tim
    auto original_quantity = GameManager::getInstance().getNumberOfHearts();
    for (int i = 0; i < original_quantity; i++)
    {
        addAHeart();
    }
    quantity = original_quantity;

    // Gán callback vẽ hay xóa từ game manager
    GameManager::getInstance().drawAHeart = [this]() { this->addAHeart(); };
    GameManager::getInstance().eraseAHeart = [this]() { this->deleteAHeart(); };
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
    // Set camera nếu không khi chương trình đang chạy mà gọi hàm thì chú ý xem node đang dùng camera nào 
    heartSprite->setCameraMask((int)(CameraFlag::USER1)); 
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
