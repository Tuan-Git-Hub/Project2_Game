#include "BackgroundMainMenu.h"
#include "SpriteManager.h"

using namespace ax;

BackgroundMainMenu* BackgroundMainMenu::createBg()
{
    BackgroundMainMenu* ret = new BackgroundMainMenu();
    if (ret && ret->init()) 
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BackgroundMainMenu::init()
{
    if (!Node::init()) 
    {
        return false;
    }
    bg1 = SpriteManager::getInstance().createSprite("backgroundMainMenu");
    bg2 = SpriteManager::getInstance().createSprite("backgroundMainMenu");
    bg3 = SpriteManager::getInstance().createSprite("frog_backgroundMainMenu");
    bg4 = SpriteManager::getInstance().createSprite("nameGame");

    bg1->setAnchorPoint(Vec2(0, 0));
    bg1->setPosition(0, 0);

    bg2->setAnchorPoint(Vec2(0, 0));
    bg2->setPosition(bg1->getContentSize().width, 0);

    bg3->setAnchorPoint(Vec2(0, 0));
    bg3->setPosition(Vec2(40, 0));
    bg3->setScale(1.8f);

    bg4->setAnchorPoint(Vec2(0, 0));
    bg4->setPosition(Vec2(30, 240));
    bg4->setScale(1.1f);


    this->addChild(bg1);
    this->addChild(bg2);
    this->addChild(bg3);
    this->addChild(bg4);

    this->scheduleUpdate();

    return true;
}

void BackgroundMainMenu::update(float dt)
{
    float moveAmount = speed * dt;

    // Di chuyển hai hình nền sang trái
    bg1->setPositionX(bg1->getPositionX() - moveAmount);
    bg2->setPositionX(bg2->getPositionX() - moveAmount);

    // Kiểm tra nếu một hình ra khỏi màn hình thì đặt lại vị trí
    if (bg1->getPositionX() <= -bg1->getContentSize().width) 
    {
        bg1->setPositionX(bg2->getPositionX() + bg2->getContentSize().width);
    }
    if (bg2->getPositionX() <= -bg2->getContentSize().width) 
    {
        bg2->setPositionX(bg1->getPositionX() + bg1->getContentSize().width);
    }
}