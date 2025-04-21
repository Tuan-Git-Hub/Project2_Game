#include "GameManager.h"
#include "SceneManager.h"
#include "SpriteManager.h"
#include "GameObjectManager.h"

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
    _scorePlayer += 1;
    drawScore(_scorePlayer);
}

// Thêm trái cây xuất hiện tại vị trí trong scene
void GameManager::spawnFruitsAt(const Vec2& po, int sl)
{
    for (int i = 0; i < sl; i++)
    {
        auto fruit = GameObjectManager::createItem(ItemType::Fruits);
        fruit->getPhysicsBody()->setDynamic(true); // bật dynamic để chịu ảnh hưởng vật lý
        fruit->getPhysicsBody()->setVelocity(Vec2(random(-200, 200), random(0, 100))); // set tốc độ và góc độ bắn ra
        Director::getInstance()->getRunningScene()->getChildByName("MapGame")->addChild(fruit); // add vào scene
        fruit->scheduleOnce([=](float) { fruit->setPosition(po); }, 0.1f, "fruits in box"); // quan trọng là phải gọi update hoặc cho vào update để ở frame sau scene nhận diện và render đúng vị trí
    }
}


// reset
void GameManager::resetGameManager()
{
    _numberOfHearts = 3;
    _scorePlayer = 0;
    _timeLeft = 9.0f;
    _pointToAddHeart = 100;
}

// Tạo scene gameover và thay thế scene
void GameManager::createSceneGameOver()
{
    SceneManager::create_and_replace_currentScene(SceneType::GameOver_Scene);
}

void GameManager::update(float dt)
{
    if (_numberOfHearts > 0)
    {    
        if (_timeLeft > 0.0f)
        {
            _timeLeft -= dt;
            drawTime(_timeLeft);
        }
        else if (_timeLeft <= 0.0f)
        {
            AXLOG("Time out");
            this->createSceneGameOver();
            return;
        }
    }
    else
    {
        AXLOG("Player die");
        this->createSceneGameOver();
        return;
    }
    if (_scorePlayer >= _pointToAddHeart)
    {
        AXLOG("Player add one heart");
        this->addAHeart();
        _pointToAddHeart += 100;
    }
}