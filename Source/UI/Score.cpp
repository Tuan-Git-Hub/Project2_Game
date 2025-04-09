#include <Score.h>
#include <SpriteManager.h>

using namespace ax;

Score* Score::instance = nullptr;

Score* Score::getInstance()
{
    if (!instance)
    {
        instance = new Score();
        if (instance && instance->init())
        {
            instance->retain(); // giữ lại giúp không tự bị xóa khi hết scene
            return instance;
        }
        delete instance;
        instance = nullptr;
        return nullptr;
    }
    return instance;
}

bool Score::init()
{
    if (!Node::init()) 
    {
        return false;
    }

    // Tạo điểm số
    scoreLabel = Label::createWithTTF("SCORE:" + std::to_string(score_player), "fonts/Matrix_Mono.ttf", 12);
    scoreLabel->setAnchorPoint(Vec2(0, 1)); // để neo ở top left
    scoreLabel->setPosition(Vec2(20, 310));
    this->addChild(scoreLabel);

    return true;
}

void Score::addPoints()
{
    score_player++;
    scoreLabel->setString("SCORE:" + std::to_string(score_player));
}

void Score::deleteInstance()
{
    if (instance)
    {
        instance->release();
        instance = nullptr;
    }
}