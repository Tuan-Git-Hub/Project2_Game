#ifndef __SCORE_H__
#define __SCORE_H__

#include "axmol.h"

class Score : public ax::Node
{
private:
    static Score* instance;
    Score() {}; // Đảm bảo rằng chỉ có thể tạo ra một instance duy nhất
    Score(const Score&) = delete; // Ngăn sao chép
    Score& operator=(const Score&) = delete; // Ngăn gán object

    int score_player = 1996;
    ax::Label* scoreLabel;
public:
    static Score* getInstance();
    bool init();

    void addPoints();
    int getScorePlayer() const { return score_player; };

    static void deleteInstance();
};

#endif  // __SCORE_H__