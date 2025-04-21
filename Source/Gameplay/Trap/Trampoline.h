#ifndef __TRAMPOLINE_H__
#define __TRAMPOLINE_H__

#include "Trap.h"

class Trampoline : public Trap
{
protected:
    ax::Action* actTrap;
public:
    Trampoline();
    ~Trampoline();
    bool init() override;

    void activateTrap() override;
    void deactivateTrap() override;
};

#endif  // !__TRAMPOLINE_H__
