#ifndef __LEVELS_BOARD_H__
#define __LEVELS_BOARD_H__

#include "axmol.h"

class LevelsBoard : public ax::Layer
{
public:
    static LevelsBoard* createLevelsBoard();
    bool init();

    void selectLevel_1();
    void selectLevel_2();
    void selectLevel_3();
    void onReturn();

};


#endif  // __LEVELS_BOARD_H__