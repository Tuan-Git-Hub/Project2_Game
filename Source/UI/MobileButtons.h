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
    bool isHoldingJump = false;

    void update(float dt);
// private:
//     void leftMove(ax::Object* sender);
//     void rightMove(ax::Object* sender);
//     void jumpMove(ax::Object* sender);
};


#endif  // __MOBILE_BUTTONS_H__