#ifndef __MOBILE_BUTTONS_H__
#define __MOBILE_BUTTONS_H__

#include "axmol.h"

class MobileButtons : public ax::Node
{
public:
    static MobileButtons* createMobileButtons();
    bool init();
    
    static std::function<void()> leftMove;
    static std::function<void()> rightMove;
    static std::function<void()> jumpMove;
    static std::function<void()> stopMove;
    
    bool isHoldingMoveLeft = false;
    bool isHoldingMoveRight = false;

    void update(float dt);
};


#endif  // __MOBILE_BUTTONS_H__