#include "GameOverBoard.h"
#include "SpriteManager.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

using namespace ax;

GameOverBoard* GameOverBoard::createGameOverBoard()
{

    GameOverBoard* ret = new GameOverBoard();
    if (ret && ret->init())
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GameOverBoard::init()
{
    if (!Layer::init())
    return false;

    // Tạo 1 board
    auto gameOverBoard = SpriteManager::getInstance().createSprite("gameOverBoard");
    gameOverBoard->setPosition(Vec2(320, 220));
    this->addChild(gameOverBoard);

    // Tạo điểm số
    auto score = GameManager::getInstance().getPlayerScore();
    auto scoreLabel_1 = Label::createWithTTF("SCORE", "fonts/Born2bSportyFS.otf", 40);
    scoreLabel_1->setAnchorPoint(Vec2(0.5, 0.5)); // để neo ở giữa
    scoreLabel_1->setPosition(Vec2(98, 62));
    scoreLabel_1->setColor(Color3B(30, 30, 50));
    gameOverBoard->addChild(scoreLabel_1);

    auto scoreLabel_2 = Label::createWithTTF(std::to_string(score), "fonts/Born2bSportyFS.otf", 40);
    scoreLabel_2->setAnchorPoint(Vec2(0.5, 0.5)); // để neo ở giữa
    scoreLabel_2->setPosition(Vec2(98, 32));
    scoreLabel_2->setColor(Color3B(30, 30, 50));
    gameOverBoard->addChild(scoreLabel_2);

    // Tạo nút
    auto restart_normal = SpriteManager::getInstance().createSprite("buttonRestartGameOver");
    auto restart_selected = SpriteManager::getInstance().createSprite("buttonRestartGameOver");
    restart_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_restart = MenuItemSprite::create(restart_normal, restart_selected, AX_CALLBACK_0(GameOverBoard::onRestart, this));

    auto exit_normal = SpriteManager::getInstance().createSprite("buttonExitGameOver");
    auto exit_selected = SpriteManager::getInstance().createSprite("buttonExitGameOver");
    exit_selected->setColor(Color3B(255, 200, 200)); // Làm tối màu
    auto button_exit = MenuItemSprite::create(exit_normal, exit_selected, AX_CALLBACK_0(GameOverBoard::onReturnMainMenu, this));

    auto buttons = Menu::create(button_restart, button_exit, nullptr);
    button_restart->setPosition(Vec2(65, -30));
    button_exit->setPosition(Vec2(130, -30));
    buttons->setPosition(Vec2::ZERO);
    gameOverBoard->addChild(buttons);

    return true;
}

// Restart game
void GameOverBoard::onRestart()
{
    AXLOG("Restart Level");
    SoundManager::playEffect(AudioPaths::CLICK);
    SceneManager::restart_currentScene();
}

// Exit
void GameOverBoard::onReturnMainMenu()
{
    AXLOG("Return Main Menu");
    SoundManager::playEffect(AudioPaths::CLICK);
    SceneManager::create_and_replace_currentScene(SceneType::MainMenu_Scene);
}