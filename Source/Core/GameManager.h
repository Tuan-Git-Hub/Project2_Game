#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "axmol.h"

class GameManager
{
private:
    GameManager() {}; // Đảm bảo rằng chỉ có thể tạo ra một instance duy nhất
    GameManager(const GameManager&) = delete; // Ngăn sao chép
    GameManager& operator=(const GameManager&) = delete; // Ngăn gán object

    int _numberOfHearts = 3;
    int _scorePlayer = 1996;
    float _timeLeft = 9.0f;
    const float time_level_1 = 15.0f;
    const float time_level_2 = 100.0f;
    const float time_level_3 = 100.0f;
    
public:
    enum class Level
        {
            LEVEL_1,
            LEVEL_2,
            LEVEL_3
        };

    static GameManager& getInstance();

    int getNumberOfHearts() const { return _numberOfHearts; };
    int getPlayerScore() const { return _scorePlayer; };
    float getTimeLeft() const { return _timeLeft; };

    void selectLevel(GameManager::Level level);
    void addAHeart();
    void minusAHeart();
    void addPointPlayer();

    void resetGameManager();
    void createSceneGameOver();
    void update(float dt);

    std::function<void()> addAHeartForPlayer;
    std::function<void()> drawAHeart;
    std::function<void()> eraseAHeart;
    std::function<void()> drawScore;
    std::function<void(float)> drawTime;
    
};


#endif  // __GAME_MANAGER_H__