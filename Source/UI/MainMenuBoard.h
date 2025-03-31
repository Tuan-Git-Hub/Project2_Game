#ifndef __MAIN_MENU_BOARD_H__
#define __MAIN_MENU_BOARD_H__

#include "axmol.h"
#include "OptionsBoard.h"
#include "LevelsBoard.h"

class MainMenuBoard : public ax::Layer
{
private:
    OptionsBoard* optionsBoard;
    LevelsBoard* levelsBoard;
public:
    static MainMenuBoard* createMainMenu();
    bool init();

    void onNewGame(ax::Object* sender);
    void onOptions(ax::Object* sender);
    void onLevels(ax::Object* sender);
    void onExit(ax::Object* sender);
};


#endif  // __MAIN_MENU_BOARD_H__