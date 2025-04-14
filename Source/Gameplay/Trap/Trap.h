#ifndef __TRAP_H__
#define __TRAP_H__

#include "axmol.h"

enum class TrapType
{
    GenericTrap = 0,
    Spike = 1,
    Fire = 2,
    Trampoline = 3,
    Saw = 4,
    SandMudIce = 5,
    Fan = 6, WindZone=61,
    RockHead = 7
};

class Trap : public ax::Sprite
{
protected:
    ax::PhysicsBody* trapBody;
    int damage;
    bool isActive;
    bool isDynamic;

public:
    Trap();
    Trap(bool _isActv, int dmg, bool _isDync);
    virtual ~Trap();

    virtual bool init() ;

    virtual void activateTrap();
    virtual void deactivateTrap();
    virtual void dealDamage(ax::Node* player);

    ax::PhysicsBody* getTrapBody();
    void setTrapBody(ax::PhysicsBody* body);

    int getDamage();
    void setDamage(int dmg);

    bool getActiveStatus();
    void setActiveStatus(bool active);

    bool getDynamicStatus();
    void setDynamicStatus(bool _dync);
};

#endif  // __TRAP_H__
