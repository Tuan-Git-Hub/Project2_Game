#ifndef __FIRE_H__
#define __FIRE_H__

#include "Trap.h"

class Fire : public Trap
{
protected:
    ax::Action* actTrap;
public:
    Fire();
    ~Fire();
    bool init() override;
 
    void activateTrap() override;
    void deactivateTrap() override;
};

#endif  // !__FIRE_H__
