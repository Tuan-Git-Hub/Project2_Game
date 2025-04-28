#include "MainMenuBoard.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

using namespace ax;

MainMenuBoard* MainMenuBoard::createMainMenu()
{
    MainMenuBoard* ret = new MainMenuBoard();
    if (ret && ret->init())
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool MainMenuBoard::init()
{
    if (!Layer::init())
        return false;

    // Tạo 1 mainmenu board
    auto mainmenuboard = SpriteManager::getInstance().createSprite("mainMenuBoard");
    mainmenuboard->setPosition(Vec2(480, 180));
    this->addChild(mainmenuboard);

    auto newgame_normal = SpriteManager::getInstance().createSprite("buttonNewgame");
    auto newgame_selected = SpriteManager::getInstance().createSprite("buttonNewgame");
    newgame_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_newgame = MenuItemSprite::create(newgame_normal, newgame_selected, AX_CALLBACK_1(MainMenuBoard::onNewGame, this));

    auto options_normal = SpriteManager::getInstance().createSprite("buttonOptions");
    auto options_selected = SpriteManager::getInstance().createSprite("buttonOptions");
    options_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_options = MenuItemSprite::create(options_normal, options_selected, AX_CALLBACK_1(MainMenuBoard::onOptions, this));

    auto levels_normal = SpriteManager::getInstance().createSprite("buttonLevels");
    auto levels_selected = SpriteManager::getInstance().createSprite("buttonLevels");
    levels_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_levels = MenuItemSprite::create(levels_normal, levels_selected, AX_CALLBACK_1(MainMenuBoard::onLevels, this));

    auto exit_normal = SpriteManager::getInstance().createSprite("buttonExit");
    auto exit_selected = SpriteManager::getInstance().createSprite("buttonExit");
    exit_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_exit = MenuItemSprite::create(exit_normal, exit_selected, AX_CALLBACK_1(MainMenuBoard::onExit, this));

    auto buttons = Menu::create(button_newgame, button_options, button_levels, button_exit, nullptr);
    buttons->alignItemsVerticallyWithPadding(12.0f);
    buttons->setPosition(Vec2(130, 97));
    
    mainmenuboard->addChild(buttons);

    // Tạo option board
    optionsBoard = OptionsBoard::createOptionsBoard();
    optionsBoard->setPosition(Vec2(1000, 1000));
    this->addChild(optionsBoard);
    

    // Tạo level board
    levelsBoard = LevelsBoard::createLevelsBoard();
    levelsBoard->setPosition(Vec2(1000, 1000));
    this->addChild(levelsBoard);

    return true;
}

void MainMenuBoard::onNewGame(Object* sender)
{
    AXLOG("New Game!!");
    SoundManager::playEffect(AudioPaths::CLICK);
    SceneManager::create_and_replace_currentScene(SceneType::Level_1_Scene);
}

void MainMenuBoard::onOptions(Object* sender)
{
    AXLOG("Options clicked");
    SoundManager::playEffect(AudioPaths::CLICK);
    this->setPosition(Vec2(1000, 1000));
    optionsBoard->setPosition(Vec2::ZERO - this->getPosition());
}

void MainMenuBoard::onLevels(Object* sender)
{
    AXLOG("Levels clicked");
    SoundManager::playEffect(AudioPaths::CLICK);
    this->setPosition(Vec2(1000, 1000));
    levelsBoard->setPosition(Vec2::ZERO - this->getPosition());
}

void MainMenuBoard::onExit(Object* sender)
{
    SoundManager::playEffect(AudioPaths::CLICK);
    _director->end();
}