#include "GameTimer.h"
#include "SpriteManager.h"
#include "fmt/core.h"

using namespace ax;

GameTimer* GameTimer::createGameTimer(Level level)
{

    GameTimer* ret = new GameTimer();
    if (ret && ret->init(level))
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GameTimer::init(Level level)
{
    if (!Node::init()) 
    {
        return false;
    }

    // Tạo 1 đồng hồ cát
    auto idleFrames = SpriteManager::getInstance().createVectorSpriteFrame("Inverted hourglass%d.png", 27);
    auto hourglass = Sprite::createWithSpriteFrame(idleFrames.front());
    auto idleAnimation = Animation::createWithSpriteFrames(idleFrames, 1.0f/10);
    auto idleAction = RepeatForever::create(Animate::create(idleAnimation));
    hourglass->runAction(idleAction);
    hourglass->setPosition(Vec2(220, 330));
    hourglass->setScale(0.7f);
    hourglass->setColor(Color3B(200, 200, 200));
    this->addChild(hourglass);

    // Tạo bộ đếm thời gian
    if (level == Level::LEVEL_1)
        timeLeft = time_level_1;
    else if (level == Level::LEVEL_2)
        timeLeft = time_level_2;
    else if (level == Level::LEVEL_3)
        timeLeft = time_level_3;

    // Tạo bộ đếm
    auto str = fmt::format("{:.0f}", timeLeft); // 1 kiểu chuyển đổi từ float về string
    timeLabel = Label::createWithTTF(str, "fonts/Matrix_Mono.ttf", 12);
    timeLabel->setAnchorPoint(Vec2(0, 0.5)); // để neo ở mid left
    timeLabel->setPosition(hourglass->getPosition() + Vec2(12, 0));
    this->addChild(timeLabel);

    scheduleUpdate();

    return true;
}

void GameTimer::update(float dt)
{
    timeLeft -= dt;
    if (timeLeft <= 0.0f)
    {
        AXLOG("time out");
        this->unscheduleUpdate();
        if (timeOut)
            timeOut();
        return;
    }
    timeLabel->setString(fmt::format("{:.0f}", timeLeft));
}