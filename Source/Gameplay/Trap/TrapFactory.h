#ifndef __TRAP_FACTORY_H__
#define __TRAP_FACTORY_H__

#include "axmol.h"
#include "Trap.h"
#include "Spike.h"
#include "Saw.h"
#include "Trampoline.h"
#include "Fire.h"
#include "RockHead.h"
#include "Fan.h"


class TrapFactory
{
public:

    static Trap* createTrap(TrapType _trapType)
    {
        switch (_trapType)
        {
            case TrapType::Spike:
            {
                Spike* certainTrap = new Spike();
                if (certainTrap && certainTrap->init())
                {
                    certainTrap->autorelease();
                    return certainTrap;
                }
                delete certainTrap;
                return nullptr;
            }
            case TrapType::Trampoline:
            {
                Trampoline* certainTrap = new Trampoline();
                if (certainTrap && certainTrap->init())
                {
                    certainTrap->autorelease();
                    return certainTrap;
                }
                delete certainTrap;
                return nullptr;
            }
            case TrapType::Fire:
            {
                Fire* certainTrap = new Fire();
                if (certainTrap && certainTrap->init())
                {
                    certainTrap->autorelease();
                    return certainTrap;
                }
                delete certainTrap;
                return nullptr;
            }
            case TrapType::Saw:
            {
                Saw* certainTrap = new Saw();
                if (certainTrap && certainTrap->init())
                {
                    certainTrap->autorelease();
                    return certainTrap;
                }
                delete certainTrap;
                return nullptr;
            }
            case TrapType::Fan:
            {
                Fan* certainTrap = new Fan();
                if (certainTrap && certainTrap->init())
                {
                    certainTrap->autorelease();
                    return certainTrap;
                }
                delete certainTrap;
                return nullptr;
            }
            case TrapType::RockHead:
            {
                RockHead* certainTrap = new RockHead();
                if (certainTrap && certainTrap->init())
                {
                    certainTrap->autorelease();
                    return certainTrap;
                }
                delete certainTrap;
                return nullptr;
            }
            default:
            {
                AXLOG("Không tạo được bẫy, trả về null pointer");
                return nullptr;
            }
        }
    }
};

#endif // __TRAP_FACTORY_H__