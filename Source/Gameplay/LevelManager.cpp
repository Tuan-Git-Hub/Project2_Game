#include "LevelManager.h"
#include "Utilities.h"
#include "MapManager.h"


using namespace ax;

LevelManager& LevelManager::getInstance()
{
    static LevelManager instance;
    return instance;
}

TMXTiledMap* LevelManager::loadLevel(Level number)
{   
    const char* filePathMap = "";
    if (number == Level::LEVEL_1)
    {
        filePathMap = "res/Map/test.1.tmx";
    }
    auto map = TMXTiledMap::create(filePathMap);
    if (!map)
    {
        Utilities::problemLoading(filePathMap);
        return nullptr;
    }
    MapManager::addPhysicBodyToTileMap(map);
    return map;
}