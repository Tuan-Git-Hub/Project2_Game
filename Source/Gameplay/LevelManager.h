#ifndef __LEVEL_MANAGER_H__
#define __LEVEL_MANAGER_H__

#include "axmol.h"
#include "Utilities.h"
#include "MapManager.h"

enum class Level
{
    LEVEL_1,
    LEVEL_2,
    LEVEL_3
};

class LevelManager
{
public:
    static ax::TMXTiledMap* loadLevel(Level number)
    {
        const char* filePathMap;
        if (number == Level::LEVEL_1)
        {
            filePathMap = "res/Map/Level1/Lv1Tilemap.tmx";
        }

        auto map = ax::TMXTiledMap::create(filePathMap);
        map->setName("MapGame");
        if (!map)
        {
            Utilities::problemLoading(filePathMap);
            return nullptr;
        }
        MapManager::addPhysicBodyToTileMap(map);
        MapManager::createObjectsFromTMXTileMap(map);
        return map;
    };
};


#endif  // __LEVEL_MANAGER_H__