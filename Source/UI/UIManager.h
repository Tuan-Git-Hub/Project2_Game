#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "axmol.h"
#include "SpriteManager.h"
#include "MainMenuBoard.h"
#include "MobileButtons.h"

class UIManager
{
public:

    static MainMenuBoard* createMainMenu()
    {
        return MainMenuBoard::createMainMenu();
    }

    static MobileButtons* createMobileButtons()
    {
        // Gọi SpriteManager để load ảnh
        SpriteManager::getInstance().loadTexture("leftButton", "res/Mobile Buttons/Mobile Buttons/3.png");
        SpriteManager::getInstance().loadTexture("rightButton", "res/Mobile Buttons/Mobile Buttons/4.png");
        SpriteManager::getInstance().loadTexture("jumpButton", "res/Mobile Buttons/Mobile Buttons/7.png");
        return MobileButtons::createMobileButtons();
    }

};




#endif  // __UI_MANAGER_H__