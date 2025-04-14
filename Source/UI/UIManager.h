#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include "axmol.h"
#include "SpriteManager.h"
#include "MainMenuBoard.h"
#include "OptionsBoard.h"
#include "MobileButtons.h"
#include "BackgroundMainMenu.h"
#include "BackgroundInGame.h"
#include "HeartLives.h"
#include "Score.h"
#include "GameTimer.h"
#include "SettingBoard.h"
#include "GameOverBoard.h"

class UIManager
{
public:
    // Gọi SpriteManager để load ảnh
    static void loadTextures_SpriteFrames()
    {
        SpriteManager::getInstance().loadTextures({
            // Back ground Main Menu
            {"backgroundMainMenu", "res/BackgroundMainMenu/background.png"},
            {"frog_backgroundMainMenu", "res/BackgroundMainMenu/frog_background.png"},
            // Back ground InGame
            {"backgroundBrown", "res/BackgroundInGame/backgroundBrown.png"},
            {"backgroundGray", "res/BackgroundInGame/backgroundGray.png"},
            {"backgroundGreen", "res/BackgroundInGame/backgroundGreen.png"},
            {"backgroundPink", "res/BackgroundInGame/backgroundPink.png"},
            {"backgroundPurple", "res/BackgroundInGame/backgroundPurple.png"},
            {"backgroundYellow", "res/BackgroundInGame/backgroundYellow.png"},
            // Main Menu Board
            {"mainMenuBoard", "res/MainMenu/MainMenu.png"},
            {"buttonNewgame", "res/MainMenu/button_newgame.png"},
            {"buttonOptions", "res/MainMenu/button_options.png"},
            {"buttonLevels", "res/MainMenu/button_levels.png"},
            {"buttonExit", "res/MainMenu/button_exit.png"},
            {"optionsBoard", "res/Options/OptionsBoard.png"},
            {"buttonOn", "res/Options/button_on.png"},
            {"buttonOff", "res/Options/button_off.png"},
            {"buttonReturn", "res/Options/button_return.png"},
            {"levelsBoard", "res/LevelsBoard/LevelsBoard.png"},
            {"level01", "res/LevelsBoard/01.png"},
            {"level02", "res/LevelsBoard/02.png"},
            {"level03", "res/LevelsBoard/03.png"},
            // Mobile buttons
            {"leftButton", "res/Mobile Buttons/Mobile Buttons/3.png"},
            {"rightButton", "res/Mobile Buttons/Mobile Buttons/4.png"},
            {"jumpButton", "res/Mobile Buttons/Mobile Buttons/7.png"},
            // Setting Board
            {"buttonSetting", "res/Options/button_setting.png"},
            {"settingBoard", "res/Options/SettingBoard_ingame.png"},
            {"buttonRestart", "res/Options/button_restart_ingame.png"},
            {"buttonExit2", "res/Options/button_exit_ingame.png"},
            // Game Over
            {"gameOverBoard", "res/GameOver/GameOverBoard.png"},
            {"buttonRestartGameOver", "res/GameOver/button_restart_GameOver.png"},
            {"buttonExitGameOver", "res/GameOver/button_exit_GameOver.png"}

        });
        // Heart lives
        SpriteManager::getInstance().loadSpriteFrames({
            "res/HeartLives/HeartIdle.plist",
            "res/Hourglass/Inverted_Hourglass.plist"
        });
    }
    static BackgroundMainMenu* createBg()
    {
        return BackgroundMainMenu::createBg();
    }

    static BackgroundInGame* createBGInGame()
    {
        return BackgroundInGame::createBGInGame();
    }


    static MainMenuBoard* createMainMenu()
    {
        return MainMenuBoard::createMainMenu();
    }

    static MobileButtons* createMobileButtons()
    {
        return MobileButtons::createMobileButtons();
    }

    static HeartLives* createHeartLives()
    {
        return HeartLives::createHeartLives();
    }

    static Score* createScoreLabel()
    {
        return Score::createScoreLabel();
    }

    static GameTimer* createGameTimer()
    {
        return GameTimer::createGameTimer();
    }

    static SettingBoard* createSettingBoardInGame()
    {
        return SettingBoard::createSettingBoard();
    }

    static GameOverBoard* createGameOverBoard()
    {
        return GameOverBoard::createGameOverBoard();
    }
};




#endif  // __UI_MANAGER_H__