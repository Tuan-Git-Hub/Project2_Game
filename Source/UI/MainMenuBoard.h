#ifndef __MAIN_MENU_BOARD_H__
#define __MAIN_MENU_BOARD_H__

#include "axmol.h"

class MainMenuBoard : public ax::Layer
{
public:
    static MainMenuBoard* createMainMenu();
    virtual bool init();

    void onNewGame(ax::Object* sender);
    void onSettings(ax::Object* sender);
    void onExit(ax::Object* sender);
};


#endif  // __MAIN_MENU_BOARD_H__