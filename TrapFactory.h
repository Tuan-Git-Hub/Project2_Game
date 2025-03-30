#pragma once
#ifndef __TRAPFACTORY_H__
#define __TRAPFACTORY_H__

#include "axmol.h"
#include <cmath>

#include "Saw.h"
#include "Spike.h"
#include "Trampoline.h"
#include "Saw.h"
#include "SandMudIce.h"
#include "Fire.h"
#include "Fan.h"

class TrapFactory
{
public:

    static Trap* createTrap(TrapType _trapType, const std::string& spriteFile);
    static Trap* createTrap(TrapType _trapType, const std::string& spriteFile, SMIType _smiType);
};

#endif
