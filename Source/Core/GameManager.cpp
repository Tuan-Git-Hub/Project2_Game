#include "GameManager.h"
#include "GameOverScene.h"
#include "SpriteManager.h"

using namespace ax;

GameManager& GameManager::getInstance()
{
    static GameManager instance;
    return instance;
}

// Chọn level
void GameManager::selectLevel(GameManager::Level level)
{
    if (level == Level::LEVEL_1)
        _timeLeft = time_level_1;
    else if (level == Level::LEVEL_2)
        _timeLeft = time_level_2;
    else if (level == Level::LEVEL_3)
        _timeLeft = time_level_3;
}

// Thêm trái tim và gọi hàm vẽ
void GameManager::addAHeart()
{
    _numberOfHearts++;
    drawAHeart();
    addAHeartForPlayer();
}

// Bớt trái tim và gọi hàm xóa
void GameManager::minusAHeart()
{
    _numberOfHearts--;
    eraseAHeart();
}

// Thêm điểm và gọi hàm vẽ
void GameManager::addPointPlayer()
{
    _scorePlayer++;
    drawScore();
}

// reset
void GameManager::resetGameManager()
{
    _numberOfHearts = 3;
    _scorePlayer = 0;
    _timeLeft = 9.0f;
}

// Tạo scene gameover và thay thế scene
void GameManager::createSceneGameOver()
{
    auto dir = Director::getInstance();
    auto gameOverScene = utils::createInstance<GameOverScene>();
    gameOverScene->setBackgroundScreenshot(SpriteManager::getInstance().getScreenshot(dir->getRunningScene()));
    dir->replaceScene(gameOverScene);
}

void GameManager::update(float dt)
{
    if (_timeLeft > 0.0f)
    {
        _timeLeft -= dt;
        drawTime(_timeLeft);
    }
    else if (_timeLeft <= 0.0f)
    {
        AXLOG("time out");
        this->createSceneGameOver();
        return;
    }
}