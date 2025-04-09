#ifndef __SETTING_BOARD_H__
#define __SETTING_BOARD_H__

#include "axmol.h"

class SettingBoard : public ax::Layer
{
private:
    ax::Menu* buttonSettingInGame;
    ax::Sprite* settingBoard;
    ax::Sprite* musicONOFF;
    ax::Sprite* sfxONOFF;

    bool isMuteMusic = false;
    bool isMuteSFX = false;

public:
    static SettingBoard* createSettingBoard();
    bool init();

    void toggleMusic();
    void toggleSFX();
    void onSetting();
    void onReturnScene();
    void onRestart();
    void onReturnMainMenu();
};

#endif  // __SETTING_BOARD_H__