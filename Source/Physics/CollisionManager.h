#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__

#include "axmol.h"

class CollisionManager
{
public:
    static void init();
    static bool onContactBegin(ax::PhysicsContact& contact);
    static bool onContactPreSolve(ax::PhysicsContact& contact, ax::PhysicsContactPreSolve& solve);
    static void onContactSeparate(ax::PhysicsContact& contact);
};




#endif  // __COLLISION_MANAGER_H__