#ifndef __OPTIONS_BOARD_H__
#define __OPTIONS_BOARD_H__

#include "axmol.h"

class OptionsBoard : public ax::Layer
{
private:   
    ax::Sprite* musicONOFF;
    ax::Sprite* sfxONOFF;

    bool isMuteMusic = false;
    bool isMuteSFX = false;

public:
    static OptionsBoard* createOptionsBoard();
    bool init();

    void toggleMusic();
    void toggleSFX();
    void onReturn();

};

#endif  // __OPTIONS_BOARD_H__