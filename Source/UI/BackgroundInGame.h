#ifndef __BACKGROUND_IN_GAME_H__
#define __BACKGROUND_IN_GAME_H__

#include "axmol.h"

class BackgroundInGame : public ax::Node
{
private:
    ax::Sprite* bg1;
    ax::Sprite* bg2;
    float speed = 30.0f;
public:
    static BackgroundInGame* createBGInGame();
    bool init();
    void update(float dt);
};


#endif  // __BACKGROUND_IN_GAME_H__