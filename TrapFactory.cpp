#include "TrapFactory.h"

USING_NS_AX;

Trap* TrapFactory::createTrap(TrapType _trapType, const std::string& spriteFile)
{
    switch (_trapType)
    {
        case TrapType::Spike:
        {
            Spike* certainTrap = new Spike();
            if (certainTrap && certainTrap->init(spriteFile))
            {
                certainTrap->autorelease();
                return certainTrap;
            }
            delete certainTrap;
            break;
        }
        case TrapType::Trampoline:
        {
            Trampoline* certainTrap = new Trampoline();
            if (certainTrap && certainTrap->init(spriteFile))
            {
                certainTrap->autorelease();
                return certainTrap;
            }
            delete certainTrap;
            break;
        }
        case TrapType::Fire:
        {
            Fire* certainTrap = new Fire();
            if (certainTrap && certainTrap->init(spriteFile))
            {
                certainTrap->autorelease();
                return certainTrap;
            }
            delete certainTrap;
            break;
        }
        case TrapType::Saw:
        {
            Saw* certainTrap = new Saw();
            if (certainTrap && certainTrap->init(spriteFile))
            {
                certainTrap->autorelease();
                return certainTrap;
            }
            delete certainTrap;
            break;
        }
        case TrapType::Fan:
        {
            Fan* certainTrap = new Fan();
            if (certainTrap && certainTrap->init(spriteFile))
            {
                certainTrap->autorelease();
                return certainTrap;
            }
            delete certainTrap;
            break;
        }
        case TrapType::RockHead:
        {
            RockHead* certainTrap = new RockHead();
            if (certainTrap && certainTrap->init(spriteFile))
            {
                certainTrap->autorelease();
                return certainTrap;
            }
            delete certainTrap;
            break;
        }
        default:
        {
            AXLOG("Không tạo được bẫy, trả về null pointer");
            return nullptr;
        }
    }
}

Trap* TrapFactory::createTrap(TrapType _trapType, const std::string& spriteFile, SMIType _smiType)
{
    if (_trapType == TrapType::SandMudIce)
    {
        SandMudIce* certainTrap = new SandMudIce(_smiType);
        if (certainTrap && certainTrap->init(spriteFile))
        {
            certainTrap->autorelease();
            return certainTrap;
        }
        delete certainTrap;
    }
    else
    {
        AXLOG("Không tạo được bẫy, trả về null pointer");
        return nullptr;
    }
}
