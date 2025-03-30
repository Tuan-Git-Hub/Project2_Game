#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include "axmol.h"
#include "Trap.h"
#include "Spike.h"
#include "Trampoline.h"
#include "Fire.h"
#include "SandMudIce.h"
#include "Fan.h"

class CollisionHandler
{
public:
    static bool onContactBegin(ax::PhysicsContact& contact);
    static bool onContactPreSolve(ax::PhysicsContact& contact, ax::PhysicsContactPreSolve& solve);
};

#endif  // COLLISION_HANDLER_H
