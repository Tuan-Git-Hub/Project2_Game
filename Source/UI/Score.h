#ifndef __SCORE_H__
#define __SCORE_H__

#include "axmol.h"

class Score : public ax::Node
{
private:
    ax::Label* scoreLabel;
public:
    static Score* createScoreLabel();
    bool init();

    void drawPoints(int point);
};

#endif  // __SCORE_H__