#ifndef __MAP_MANAGER_H__
#define __MAP_MANAGER_H__

#include "axmol.h"

class MapManager
{
public:
    static void addPhysicBodyToTileMap(ax::TMXTiledMap* map); // tạo physicbody cho các layer
    static void createObjectsFromTMXTileMap(ax::TMXTiledMap* map); // lấy thông tin về các object và tạo
    static void triggerHiddenMap(ax::Node* node); // Kích hoạt map ẩn
};

#endif  // __MAP_MANAGER_H__