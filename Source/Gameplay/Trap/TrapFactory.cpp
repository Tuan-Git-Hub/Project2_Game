#include "TrapFactory.h"
#include <cmath>
#include "Spike.h"
// #include "Saw.h"
// #include "Trampoline.h"
// #include "Saw.h"
// #include "SandMudIce.h"
// #include "Fire.h"
// #include "Fan.h"
// #include "RockHead.h"

using namespace ax;

Trap* TrapFactory::createTrap(TrapType _trapType)
{
    switch (_trapType)
    {
        case TrapType::Spike:
        {
            Trap* certainTrap = new Spike();
            if (certainTrap && certainTrap->init())
            {
                certainTrap->autorelease();
                return certainTrap;
            }
            delete certainTrap;
            return nullptr;
        }
        // case TrapType::Trampoline:
        // {
        //     Trampoline* certainTrap = new Trampoline();
        //     certainTrap->init(spriteFile);
        //     return certainTrap;
        //     break;
        // }
        // case TrapType::Fire:
        // {
        //     Fire* certainTrap = new Fire();
        //     certainTrap->init(spriteFile);
        //     return certainTrap;
        //     break;
        // }
        // case TrapType::Saw:
        // {
        //     Saw* certainTrap = new Saw();
        //     certainTrap->init(spriteFile);
        //     return certainTrap;
        //     break;
        // }
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
}