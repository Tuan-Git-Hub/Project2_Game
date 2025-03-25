#include "MainMenuBoard.h"
#include "Utilities.h"
#include "Level_1_Scene.h"

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

    // Tạo nền hình chữ nhật (menu board)
    auto bg = Sprite::create();
    bg->setTextureRect(Rect(0, 0, 300, 200));
    bg->setColor(Color3B(50, 50, 50));  // Màu nền
    bg->setOpacity(200);
    bg->setAnchorPoint(Vec2(0.5f, 0.5f));  // Căn giữa
    bg->setPosition(640, 360);
    this->addChild(bg);

    // Tạo MenuItems
    auto labelNewGame = Label::createWithSystemFont("New Game", "fonts/Marker Felt.ttf", 24);
    auto menuItemNewGame = MenuItemLabel::create(labelNewGame, AX_CALLBACK_1(MainMenuBoard::onNewGame, this));

    auto labelSettings = Label::createWithSystemFont("Settings", "fonts/Marker Felt.ttf", 24);
    auto menuItemSettings = MenuItemLabel::create(labelSettings, AX_CALLBACK_1(MainMenuBoard::onSettings, this));

    auto labelExit = Label::createWithSystemFont("Exit", "fonts/Marker Felt.ttf", 24);
    auto menuItemExit = MenuItemLabel::create(labelExit, AX_CALLBACK_1(MainMenuBoard::onExit, this));

    // Tạo Menu và căn chỉnh vị trí
    auto menu = Menu::create(menuItemNewGame, menuItemSettings, menuItemExit, nullptr);
    menu->alignItemsVerticallyWithPadding(15); // Tạo khoảng cách giữa các label
    menu->setPosition(150, 100);
    bg->addChild(menu);

    return true;
}

void MainMenuBoard::onNewGame(Object* sender)
{
    AXLOG("Frist Scene");
    auto firstScene = utils::createInstance<Level_1_Scene>();
    _director->replaceScene(firstScene);
}

void MainMenuBoard::onSettings(Object* sender)
{
    AXLOG("Settings clicked");
}

void MainMenuBoard::onExit(Object* sender)
{
    _director->end();
}