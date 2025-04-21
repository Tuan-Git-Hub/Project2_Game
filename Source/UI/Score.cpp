#include "Score.h"
#include "SpriteManager.h"
#include "GameManager.h"

using namespace ax;

Score* Score::createScoreLabel()
{

    Score* ret = new Score();
    if (ret && ret->init())
    {
        ret->autorelease(); // quản lý bộ nhớ tự động, giúp không cần delete thủ công của axmol
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Score::init()
{
    if (!Node::init()) 
    {
        return false;
    }
    // Lấy thông tin điểm số
    auto score_player = GameManager::getInstance().getPlayerScore();
    // Tạo điểm số
    scoreLabel = Label::createWithTTF("SCORE:" + std::to_string(score_player), "fonts/Matrix_Mono.ttf", 12);
    scoreLabel->setAnchorPoint(Vec2(0, 1)); // để neo ở top left
    scoreLabel->setPosition(Vec2(20, 310));
    this->addChild(scoreLabel);

    // Gán callback vẽ từ game manager
    GameManager::getInstance().drawScore = [this](int p) { this->drawPoints(p); };
    return true;
}

void Score::drawPoints(int point)
{
    scoreLabel->setString("SCORE:" + std::to_string(point));
}
