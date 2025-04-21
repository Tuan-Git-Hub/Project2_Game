#include "GameOverScene.h"
#include "UIManager.h"

using namespace ax;

bool GameOverScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin      = _director->getVisibleOrigin();
    auto safeArea    = _director->getSafeAreaRect();
    auto safeOrigin  = safeArea.origin;

    auto gameOverBoard = UIManager::createGameOverBoard();
    this->addChild(gameOverBoard);

    scheduleUpdate();

    return true;
}

void GameOverScene::setBackgroundScreenshot(Sprite* screenshot)
{
    screenshot->setPosition(Vec2(0, 0));
    screenshot->setOpacity(50);
    screenshot->setAnchorPoint(Vec2::ZERO);
    this->addChild(screenshot, -1); // add vào nền
}

void GameOverScene::update(float delta)
{
    switch (_gameState)
    {
        case GameState::init:
        {
            _gameState = GameState::update;
            break;
        }

        case GameState::update:
        {
            break;
        }
    }
}

GameOverScene::GameOverScene()
{
    AXLOG("Scene: GameOver ctor");
}

GameOverScene::~GameOverScene()
{
    AXLOG("~Scene: GameOver dtor");
}
