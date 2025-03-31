#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "axmol.h"
#include "SpriteManager.h"
#include "MainMenuBoard.h"
#include "OptionsBoard.h"
#include "MobileButtons.h"
#include "BackgroundMainMenu.h"

class UIManager
{
public:
    static BackgroundMainMenu* createBg()
    {
        SpriteManager::getInstance().loadTexture("backgroundMainMenu", "res/BackgroundMainMenu/background.png");
        SpriteManager::getInstance().loadTexture("frog_backgroundMainMenu", "res/BackgroundMainMenu/frog_background.png");
        return BackgroundMainMenu::createBg();
    }

    static MainMenuBoard* createMainMenu()
    {
        SpriteManager::getInstance().loadTexture("mainMenuBoard", "res/MainMenu/MainMenu.png");
        SpriteManager::getInstance().loadTexture("buttonNewgame", "res/MainMenu/button_newgame.png");
        SpriteManager::getInstance().loadTexture("buttonOptions", "res/MainMenu/button_options.png");
        SpriteManager::getInstance().loadTexture("buttonLevels", "res/MainMenu/button_levels.png");
        SpriteManager::getInstance().loadTexture("buttonExit", "res/MainMenu/button_exit.png");

        SpriteManager::getInstance().loadTexture("optionsBoard", "res/Options/OptionsBoard.png");
        SpriteManager::getInstance().loadTexture("buttonOn", "res/Options/button_on.png");
        SpriteManager::getInstance().loadTexture("buttonOff", "res/Options/button_off.png");
        SpriteManager::getInstance().loadTexture("buttonReturn", "res/Options/button_return.png");

        SpriteManager::getInstance().loadTexture("levelsBoard", "res/LevelsBoard/LevelsBoard.png");
        SpriteManager::getInstance().loadTexture("level01", "res/LevelsBoard/01.png");
        SpriteManager::getInstance().loadTexture("level02", "res/LevelsBoard/02.png");
        SpriteManager::getInstance().loadTexture("level03", "res/LevelsBoard/03.png");
        return MainMenuBoard::createMainMenu();
    }

    // static OptionsBoard* createOptionsBoard()
    // {
    //     SpriteManager::getInstance().loadTexture("optionsBoard", "res/Options/OptionsBoard.png");
    //     SpriteManager::getInstance().loadTexture("buttonOn", "res/Options/button_on.png");
    //     SpriteManager::getInstance().loadTexture("buttonOff", "res/Options/button_off.png");
    //     SpriteManager::getInstance().loadTexture("buttonReturn", "res/Options/button_return.png");
    //     return OptionsBoard::createOptionsBoard();
    // }

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