#ifndef __TRAP_FACTORY_H__
#define __TRAP_FACTORY_H__

#include "axmol.h"
#include "Trap.h"
#include "Spike.h"
#include "Saw.h"
#include "Trampoline.h"
// #include "Saw.h"
// #include "SandMudIce.h"
// #include "Fire.h"
// #include "Fan.h"
// #include "RockHead.h"

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
            // case TrapType::Fire:
            // {
            //     Fire* certainTrap = new Fire();
            //     certainTrap->init(spriteFile);
            //     return certainTrap;
            //     break;
            // }
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
            // case TrapType::Fan:
            // {
            //     Fan* certainTrap = new Fan();
            //     certainTrap->init(spriteFile);
            //     return certainTrap;
            //     break;
            // }
            // case TrapType::RockHead:
            // {
            //     RockHead* certainTrap = new RockHead();
            //     certainTrap->init(spriteFile);
            //     return certainTrap;
            //     break;
            // }
            default:
            {
                AXLOG("Không tạo được bẫy, trả về null pointer");
                return nullptr;
            }
        }
    };


    
    // static Trap* createTrap(TrapType _trapType, SMIType _smiType);
};

#endif // __TRAP_FACTORY_H__