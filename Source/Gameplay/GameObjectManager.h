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
    static const uint16_t None = 0x000;
    static const uint16_t Ground = 0x001;
    static const uint16_t Player = 0x002;
    static const uint16_t SpikeTrap = 0x004;
    static const uint16_t SawTrap = 0x008;
    static const uint16_t Trampoline = 0x010;
    static const uint16_t Sand = 0x020;
    static const uint16_t Mud = 0x040;
    static const uint16_t Ice = 0x080;
    static const uint16_t Box = 0x100;
    static const uint16_t Box_Fragment = 0x200;
    static const uint16_t Fruits = 0x400;
    static const uint16_t Trigger = 0x800;
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
            // Box
            { "Box1_Idle", "res/Items/Boxes/Box1/Box1_Idle.png" },
            { "Box2_Idle", "res/Items/Boxes/Box2/Box2_Idle.png" },
            { "Box3_Idle", "res/Items/Boxes/Box3/Box3_Idle.png" },
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