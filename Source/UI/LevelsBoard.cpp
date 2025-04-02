#include "LevelsBoard.h"
#include "SpriteManager.h"
#include "MainMenuBoard.h"
#include "Level_1_Scene.h"

using namespace ax;

LevelsBoard* LevelsBoard::createLevelsBoard()
{

    LevelsBoard* ret = new LevelsBoard();
    if (ret && ret->init())
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool LevelsBoard::init()
{
    if (!Layer::init())
    return false;

    // Tạo 1 level board
    auto levelsboard = SpriteManager::getInstance().createSprite("levelsBoard");
    levelsboard->setPosition(Vec2(480, 195));
    this->addChild(levelsboard);
    
    // Tạo các màn
    auto level01_normal = SpriteManager::getInstance().createSprite("level01");
    auto level01_selected = SpriteManager::getInstance().createSprite("level01");
    level01_selected->setScale(1.1f); // làm to hơn khi chọn
    auto button_select_level01 = MenuItemSprite::create(level01_normal, level01_selected, AX_CALLBACK_0(LevelsBoard::selectLevel_1, this));
    button_select_level01->setScale(1.8f);

    auto level02_normal = SpriteManager::getInstance().createSprite("level02");
    auto level02_selected = SpriteManager::getInstance().createSprite("level02");
    level02_selected->setScale(1.1f);
    auto button_select_level02 = MenuItemSprite::create(level02_normal, level02_selected, AX_CALLBACK_0(LevelsBoard::selectLevel_2, this));
    button_select_level02->setScale(1.8f);

    auto level03_normal = SpriteManager::getInstance().createSprite("level03");
    auto level03_selected = SpriteManager::getInstance().createSprite("level03");
    level03_selected->setScale(1.1f);
    auto button_select_level03 = MenuItemSprite::create(level03_normal, level03_selected, AX_CALLBACK_0(LevelsBoard::selectLevel_3, this));
    button_select_level03->setScale(1.8f);


    auto return_normal = SpriteManager::getInstance().createSprite("buttonReturn");
    auto return_selected = SpriteManager::getInstance().createSprite("buttonReturn");
    return_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_return = MenuItemSprite::create(return_normal, return_selected, AX_CALLBACK_0(LevelsBoard::onReturn, this));

    auto buttons = Menu::create(button_select_level01, button_select_level02, button_select_level03, button_return, nullptr);
    button_select_level01->setPosition(Vec2(50, 85));
    button_select_level02->setPosition(Vec2(110, 85));
    button_select_level03->setPosition(Vec2(50, 45));
    button_return->setPosition(Vec2(10, 10));
    buttons->setPosition(Vec2::ZERO);
    levelsboard->addChild(buttons);

    return true;
}

void LevelsBoard::selectLevel_1()
{
    AXLOG("Scene Level 1");
    auto firstScene = utils::createInstance<Level_1_Scene>();
    _director->replaceScene(firstScene);
}

void LevelsBoard::selectLevel_2()
{
    AXLOG("Scene Level 2");
}

void LevelsBoard::selectLevel_3()
{
    AXLOG("Scene Level 3");
}

void LevelsBoard::onReturn()
{
    AXLOG("Return MainMenu Board");
    this->setPosition(Vec2(1000, 1000));
    MainMenuBoard* mainMenuBoard = dynamic_cast<MainMenuBoard*>(this->getParent());
    mainMenuBoard->setPosition(Vec2::ZERO);
}