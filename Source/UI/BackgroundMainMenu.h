#ifndef __BACKGROUND_MAIN_MENU_H__
#define __BACKGROUND_MAIN_MENU_H__

#include "axmol.h"

class BackgroundMainMenu : public ax::Node
{
private:
    ax::Sprite* bg1;
    ax::Sprite* bg2;
    ax::Sprite* bg3;
    ax::Sprite* bg4;
    float speed = 100.0f;
public:
    static BackgroundMainMenu* createBg();
    bool init();
    void update(float dt);
};


#endif  // __BACKGROUND_MAIN_MENU_H__