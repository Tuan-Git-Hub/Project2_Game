#ifndef __SCORE_H__
#define __SCORE_H__

#include "axmol.h"

class Score : public ax::Node
{
private:
    int score_player = 1996;
    ax::Label* scoreLabel;
public:
    static Score* createScoreLabel();
    bool init();

    void addPoints();
    int getScorePlayer() const { return score_player; };

};

#endif  // __SCORE_H__