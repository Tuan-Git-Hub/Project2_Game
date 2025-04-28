#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "axmol.h"
#include "SpriteManager.h"
#include "Player.h"
#include "TrapFactory.h"
#include "ItemFactory.h"

// Tạo 1 danh sách các bitmask sẽ gán. dùng uint8_t (unsigned integer) để tiết kiệm bộ nhớ 
struct ObjectBitmask
{
    static const uint32_t None = 0x000;
    static const uint32_t Ground = 0x001;
    static const uint32_t Player = 0x002;
    static const uint32_t SpikeTrap = 0x004;
    static const uint32_t SawTrap = 0x008;
    static const uint32_t Trampoline = 0x010;
    static const uint32_t Sand = 0x020;
    static const uint32_t Mud = 0x040;
    static const uint32_t Ice = 0x080;
    static const uint32_t Box = 0x100;
    static const uint32_t Box_Fragment = 0x200;
    static const uint32_t Fruits = 0x400;
    static const uint32_t Trigger = 0x800;
    static const uint32_t ChechpointFlag = 0x1000;
    static const uint32_t FireTrap = 0x2000;
    static const uint32_t RockHead = 0x4000;
    static const uint32_t Fan = 0x8000;
    static const uint32_t WinZone = 0x10000;
};

class GameObjectManager
{
public:
    // Gọi SpriteManager để load ảnh
    static void loadTextures_SpriteFrames()
    {
        // Load ảnh png vào texture cache
        SpriteManager::getInstance().loadTextures({
            // Player
            { "playerJump", "res/Player/Ninja Frog/Player_1_Jump.png" },
            { "playerFall", "res/Player/Ninja Frog/Player_1_Fall.png" },
            // Spike
            { "spikeTrap", "res/Traps/Spikes/Spike_Idle.png" },
            // Saw
            { "sawTrap", "res/Traps/Saw/Saw_Off.png" },
            // Trampoline
            { "trampolineTrap", "res/Traps/Trampoline/Trampoline_Idle.png" },
            // Fire
            { "fireTrap", "res/Traps/Fire/Fire_Off.png" },
            // Rock Head
            { "rockHead", "res/Traps/Rock Head/RockHead_Idle.png" },
            // Fan
            { "fan", "res/Traps/Fan/Fan_Off.png" },
            // Dust Particle
            { "dustParticle", "res/Traps/Fan/Dust Particle.png" },
            // Box
            { "Box1_Idle", "res/Items/Boxes/Box1/Box1_Idle.png" },
            { "Box2_Idle", "res/Items/Boxes/Box2/Box2_Idle.png" },
            { "Box3_Idle", "res/Items/Boxes/Box3/Box3_Idle.png" },
            // Checkpoint
            { "Checkpoint_NoFlag", "res/Items/Checkpoints/Checkpoint/Checkpoint(NoFlag).png" },
        });

        // Load plist vào SpriteFramesCache  
        SpriteManager::getInstance().loadSpriteFrames({ 
            // Player
            "res/Player/Ninja Frog/Idle/Player_1_Idle.plist", 
            "res/Player/Ninja Frog/Run/Player_1_Run.plist", 
            "res/Player/Ninja Frog/DoubleJump/Player_1_DoubleJump.plist", 
            "res/Player/Ninja Frog/WallJump/Player_1_WallJump.plist", 
            "res/Player/Ninja Frog/Hit/Player_1_Hit.plist",
            // Saw
            "res/Traps/Saw/Saw_On.plist",
            // Trampoline
            "res/Traps/Trampoline/Trampoline_Jump.plist",
            // Fire
            "res/Traps/Fire/Fire_On.plist",
            // Rock Head
            "res/Traps/Rock Head/RockHead_Blink.plist",
            "res/Traps/Rock Head/RockHead_BottomHit.plist",
            "res/Traps/Rock Head/RockHead_TopHit.plist",
            "res/Traps/Rock Head/RockHead_LeftHit.plist",
            "res/Traps/Rock Head/RockHead_RightHit.plist",
            // Fan
            "res/Traps/Fan/Fan_On.plist",
            // Box
            "res/Items/Boxes/Box1/Box1_Hit.plist",
            "res/Items/Boxes/Box1/Box1_Break.plist",
            "res/Items/Boxes/Box2/Box2_Hit.plist",
            "res/Items/Boxes/Box2/Box2_Break.plist",
            "res/Items/Boxes/Box3/Box3_Hit.plist",
            "res/Items/Boxes/Box3/Box3_Break.plist",
            // Fruits
            "res/Items/Fruits/Fruits_Collected.plist",
            "res/Items/Fruits/Apple/Apple.plist",
            "res/Items/Fruits/Bananas/Bananas.plist",
            "res/Items/Fruits/Cherries/Cherries.plist",
            "res/Items/Fruits/Kiwi/Kiwi.plist",
            "res/Items/Fruits/Melon/Melon.plist",
            "res/Items/Fruits/Orange/Orange.plist",
            "res/Items/Fruits/Pineapple/Pineapple.plist",
            "res/Items/Fruits/Strawberry/Strawberry.plist",
            // Checkpoint
            "res/Items/Checkpoints/Checkpoint/Checkpoint_FlagOut.plist",
            "res/Items/Checkpoints/Checkpoint/Checkpoint_FlagIdle.plist",
        });
    }

    static Player* createPlayer()
    {
        return Player::create();
    }

    static Trap* createTrap(TrapType type)
    {
        return TrapFactory::createTrap(type);
    }

    static Item* createItem(ItemType type)
    {
        return ItemFactory::createItem(type);
    }

};

#endif  // __GAME_OBJECT_MANAGER_H__