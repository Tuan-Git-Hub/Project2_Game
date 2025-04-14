#ifndef __TRAP_FACTORY_H__
#define __TRAP_FACTORY_H__

#include "axmol.h"
#include "Trap.h"

class TrapFactory
{
public:

    static Trap* createTrap(TrapType _trapType);
    // static Trap* createTrap(TrapType _trapType, SMIType _smiType);
};

#endif // __TRAP_FACTORY_H__