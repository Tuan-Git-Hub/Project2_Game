#ifndef __FAN_H__
#define __FAN_H__

#include "Trap.h"


class Fan : public Trap
{
protected:
    ax::Action* actTrap;
    ax::Node* winZone;

public:
    Fan();
    ~Fan();
    bool init() override;

    void activateTrap() override;
    void deactivateTrap() override;

};

#endif  // !__FAN_H__
