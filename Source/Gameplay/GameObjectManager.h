#ifndef __GAME_OBJECT_MANAGER_H__
#define __GAME_OBJECT_MANAGER_H__

#include "axmol.h"
#include "SpriteManager.h"
#include "Player.h"
#include "TrapFactory.h"

// Tạo 1 danh sách các bitmask sẽ gán. dùng uint8_t (unsigned integer) để tiết kiệm bộ nhớ 
struct ObjectBitmask
{
    static const uint8_t None = 0x00;
    static const uint8_t Ground = 0x01;
    static const uint8_t Player = 0x02;
    static const uint8_t SpikeTrap = 0x03;
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
            { "spikeTrap", "res/Traps/Spikes/Spike_Idle.png" }
        });

        // Load plist vào SpriteFramesCache  
        SpriteManager::getInstance().loadSpriteFrames({ 
            // Player
            "res/Player/Ninja Frog/Idle/Player_1_Idle.plist", 
            "res/Player/Ninja Frog/Run/Player_1_Run.plist", 
            "res/Player/Ninja Frog/DoubleJump/Player_1_DoubleJump.plist", 
            "res/Player/Ninja Frog/WallJump/Player_1_WallJump.plist", 
            "res/Player/Ninja Frog/Hit/Player_1_Hit.plist"
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
};





#endif  // __GAME_OBJECT_MANAGER_H__