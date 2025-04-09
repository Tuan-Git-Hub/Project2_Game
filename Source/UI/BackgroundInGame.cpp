#include "BackgroundInGame.h"
#include "SpriteManager.h"

using namespace ax;

BackgroundInGame* BackgroundInGame::createBGInGame()
{
    BackgroundInGame* ret = new BackgroundInGame();
    if (ret && ret->init()) 
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BackgroundInGame::init()
{
    if (!Node::init()) 
    {
        return false;
    }
    // 6 loại background
    const std::string& bgName1 = "backgroundBrown";
    const std::string& bgName2 = "backgroundGray";
    const std::string& bgName3 = "backgroundGreen";
    const std::string& bgName4 = "backgroundPink";
    const std::string& bgName5 = "backgroundPurple";
    const std::string& bgName6 = "backgroundYellow";
    std::string bgInGame;
    // Tạo random lấy 1
    int ranInt = RandomHelper::random_int(1, 6);
    AXLOG("%d", ranInt);
    switch(ranInt)
    {
        case 1:
            bgInGame = bgName1;
            break;
        case 2:
            bgInGame = bgName2;
            break;
        case 3:
            bgInGame = bgName3;
            break;
        case 4:
            bgInGame = bgName4;
            break;
        case 5:
            bgInGame = bgName5;
            break;
        case 6:
            bgInGame = bgName6;
            break;
    }
    bg1 = SpriteManager::getInstance().createSprite(bgInGame);
    bg2 = SpriteManager::getInstance().createSprite(bgInGame);
    bg1->setAnchorPoint(Vec2(0, 0));
    bg2->setAnchorPoint(Vec2(0, 0));
    bg1->setPosition(0, 0);
    bg2->setPosition(0, bg1->getContentSize().height);

    this->addChild(bg1);
    this->addChild(bg2);
    this->scheduleUpdate();
    return true;
}

void BackgroundInGame::update(float dt)
{
    float moveAmount = speed * dt;

    // Di chuyển hai hình nền xuống dưới
    bg1->setPositionY(bg1->getPositionY() - moveAmount);
    bg2->setPositionY(bg2->getPositionY() - moveAmount);

    // Kiểm tra nếu một hình ra khỏi màn hình thì đặt lại vị trí
    if (bg1->getPositionY() <= -bg1->getContentSize().height) 
    {
        bg1->setPositionY(bg2->getPositionY() + bg2->getContentSize().height);
    }
    if (bg2->getPositionY() <= -bg2->getContentSize().height) 
    {
        bg2->setPositionY(bg1->getPositionY() + bg1->getContentSize().height);
    }
}