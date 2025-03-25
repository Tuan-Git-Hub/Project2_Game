#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__

#include "axmol.h"

class LevelManager
{
public:
    enum class Level
    {
        LEVEL_1,
        LEVEL_2,
        LEVEL_3
    };

    static LevelManager& getInstance(); // Lấy singleton 


    ax::TMXTiledMap* loadLevel(Level number);
};


#endif  // __LEVEL_MANAGER_H__