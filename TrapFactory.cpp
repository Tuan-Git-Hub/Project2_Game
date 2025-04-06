#include "TrapFactory.h"

USING_NS_AX;

Trap* TrapFactory::createTrap(TrapType _trapType, const std::string& spriteFile)
{
    switch (_trapType)
    {
        case TrapType::Spike:
        {
            Spike* certainTrap = new Spike();
            certainTrap->init(spriteFile);
            return certainTrap;
            break;
        }
        case TrapType::Trampoline:
        {
            Trampoline* certainTrap = new Trampoline();
            certainTrap->init(spriteFile);
            return certainTrap;
            break;
        }
        case TrapType::Fire:
        {
            Fire* certainTrap = new Fire();
            certainTrap->init(spriteFile);
            return certainTrap;
            break;
        }
        case TrapType::Saw:
        {
            Saw* certainTrap = new Saw();
            certainTrap->init(spriteFile);
            return certainTrap;
            break;
        }
        case TrapType::Fan:
        {
            Fan* certainTrap = new Fan();
            certainTrap->init(spriteFile);
            return certainTrap;
            break;
        }
        case TrapType::RockHead:
        {
            RockHead* certainTrap = new RockHead();
            certainTrap->init(spriteFile);
            return certainTrap;
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
        certainTrap->init(spriteFile);
        return certainTrap;
    }
    else
    {
        AXLOG("Không tạo được bẫy, trả về null pointer");
        return nullptr;
    }
}
